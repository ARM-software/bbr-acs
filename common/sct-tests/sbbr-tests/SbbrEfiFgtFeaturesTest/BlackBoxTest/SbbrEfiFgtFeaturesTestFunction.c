/** @file

  Copyright 2006-2012 Unified EFI, Inc.<BR>
  Copyright (c) 2021-2024, Arm Ltd, All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at 
  http://opensource.org/licenses/bsd-license.php
 
  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
 
**/
/*++

Module Name:
 SbbrEfiFgtFeaturesTestFunction.c

Abstract:
  Source file for SBBR EFI FGT features Black-Box Test - Function Test.

--*/

#include "SctLib.h"
#include "SbbrEfiFgtFeaturesTestMain.h"
#include "AArch64/PeSysReg.h"

static EFI_CPU_ARCH_PROTOCOL   *gCpu = NULL;
static void  *gBranchToTest = NULL;
static UINT32 gSyncIntReceived = 0;
static UINT32 gTimeout = 0x100000;

static UINT32 gFgtTestStatus = FGT_TEST_COMPLETED;
static UINT32 gFgtTestStatusSize = sizeof(UINT32);

static UINT64 gStackPointer = 0;
static UINT64 gExceptionRetAddr = 0;
static UINT64 gRetAddr = 0;

//
// Prototypes (external)
//

EFI_STATUS
SbbrEfiFgtFeaturesTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// Prototypes (internal)
//

EFI_STATUS
SbbrEfiFgtFeaturesTestSub1 (
  IN EFI_RUNTIME_SERVICES                 *RT,
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib,
  IN EFI_TEST_LOGGING_LIBRARY_PROTOCOL    *LoggingLib
  );

//
// Prototypes (internal)
//

EFI_STATUS
WatchdogTimerEnableAndDisable (
  IN UINTN  Timeout
  );

//
// Prototypes (internal)
//

EFI_STATUS
GetAndSetFgtStatusVariable (
  IN UINT8    Type,
  IN UINT32  *Value,
  IN UINT32  *Size
  );

//
// Prototypes (internal)
//

VOID
PeContextSave (
  IN UINT64  Sp,
  IN UINT64  Elr
  );

//
// Prototypes (internal)
//

VOID
PeUpdateElr (
  IN VOID    *Context,
  IN UINT64  Offset
  );

//
// Prototypes (internal)
//

UINT64
PeGetEsr (
  IN VOID *Context
  );

//
// Prototypes (internal)
//

UINT64
PeGetFar (
  IN VOID *Context
  );

//
// Prototypes (internal)
//

VOID
SynchrnousExceptionHandler (
  IN UINT64  InterruptType,
  IN VOID    *Context
  );

//
// Prototypes (internal)
//

EFI_STATUS
RegisterSynchronousExceptionHandler (
  IN UINT32  ExceptionType,
  IN VOID    (*ExceptionHandler)(UINT64, VOID *)
  );

//
// Prototypes (internal)
//

VOID
CheckFeatFgtTrapAccess (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib
  );

 /* SbbrEfiFgtFeaturesTest - Entry point for SbbrEfiFgtFeaturesTest Function Test.
 *  @param This             A pointer to the EFI_BB_TEST_PROTOCOL instance.
 *  @param ClientInterface  A pointer to the interface to be tested.
 *  @param TestLevel        Test "thoroughness" control.
 *  @param SupportHandle    A handle containing support protocols.
 *  @return EFI_SUCCESS     Successfully.
 *  @return Other value     Something failed.
 */
EFI_STATUS
SbbrEfiFgtFeaturesTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STATUS                          Status;
  EFI_RUNTIME_SERVICES                *RT;
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL  *StandardLib;
  EFI_TEST_RECOVERY_LIBRARY_PROTOCOL  *RecoveryLib;
  EFI_TEST_LOGGING_LIBRARY_PROTOCOL   *LoggingLib;

  //
  // Get test support library interfaces
  //
  Status = GetTestSupportLibrary (
             SupportHandle,
             &StandardLib,
             &RecoveryLib,
             &LoggingLib
             );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  if (FALSE == CheckBBTestCanRunAndRecordAssertion(
                  StandardLib, 
                  L"SBBR SbbrEfiFgtFeaturesTest_func - Failed to test FGT features",
                  __FILE__,
                  (UINTN)__LINE__
                  )) {
    return EFI_SUCCESS;
  }

  RT = (EFI_RUNTIME_SERVICES *)ClientInterface;

  Status = SbbrEfiFgtFeaturesTestSub1 (RT, StandardLib, LoggingLib);

  return EFI_SUCCESS;
}

 /* SbbrEfiFgtFeaturesTestSub1  - Entry point for SbbrEfiFgtFeatures Sub Function Test.
 *  @param RT             A pointer to the Runtime services.
 *  @param StandardLib    A pointer to the Standard library.
 *  @param LoggingLib     A pointer to the Logging library.
 *  @return EFI_SUCCESS     Successfully.
 *  @return Other value     Something failed.
 */
EFI_STATUS
SbbrEfiFgtFeaturesTestSub1 (
  IN EFI_RUNTIME_SERVICES                 *RT,
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib,
  IN EFI_TEST_LOGGING_LIBRARY_PROTOCOL    *LoggingLib
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 FgtReg = 0;
  UINT64 Mmfr0Reg = 0;

  //
  // Trace ...
  //
  if (LoggingLib != NULL) {
    LoggingLib->EnterFunction (
                  LoggingLib,
                  L"SbbrEfiFgtFeaturesTestSub1",
                  L"SBBR "
                  );
  }

  Status = GetAndSetFgtStatusVariable(GET_VARIABLE, &gFgtTestStatus, &gFgtTestStatusSize);
  if (EFI_ERROR (Status)) {
    SctPrint(L"Failed to read nvram fgt test status, Status - %r\n", Status);
  }

  if (gFgtTestStatus == FGT_TEST_IS_IN_PROGRESS) {
    gFgtTestStatus = FGT_TEST_COMPLETED;
    Status = GetAndSetFgtStatusVariable(SET_VARIABLE, &gFgtTestStatus, &gFgtTestStatusSize);
    if (EFI_ERROR (Status)) {
      SctPrint(L"Failed to write nvram fgt test status, Status - %r\n", Status);
    }
    StandardLib->RecordAssertion (
                  StandardLib,
                  EFI_TEST_ASSERTION_FAILED,
                  gSbbrEfiTestEL3FirmwareUnsupportGuid,
                  L"EL3 F/W do not support both firmware trap and FEAT_FGT/FEAT_FGT2 features ",
                  L"%a:%d:Status - %r",
                  __FILE__,
                  (UINTN)__LINE__,
                  Status
                  );
  }
  else {
    Mmfr0Reg = AA64ReadMmfr0();
    SctPrint(L"Mmfr0Reg = 0x%llx \n", Mmfr0Reg);
    FgtReg = VAL_EXTRACT_BITS(Mmfr0Reg, 56, 59);
    SctPrint(L"FEAT_FGT = 0x%x \n", FgtReg);

    /* Check whether PE support FEAT_FGT/FEAT_FGT2 or not*/
    if ((FgtReg == 0x1) || (FgtReg == 0x10)) {
      CheckFeatFgtTrapAccess(StandardLib);
    }
    else {
      StandardLib->RecordAssertion (
                  StandardLib,
                  EFI_TEST_ASSERTION_WARNING,
                  gSbbrEfiTestPeUnsupportGuid,
                  L"PE does not support FEAT_FGT/FEAT_FGT2 ",
                  L"%a:%d:Status - %r",
                  __FILE__,
                  (UINTN)__LINE__,
                  Status
                  );
    }
  }

  //
  // Trace ...
  //
  if (LoggingLib != NULL) {
    LoggingLib->ExitFunction (
                  LoggingLib,
                  L"SbbrEfiFgtFeaturesTestSub1",
                  L"SBBR"
                  );
  }

  //
  // Done
  //
  return EFI_SUCCESS;
}

 /* WatchdogTimerEnableAndDisable  - To enable ro disable Watchdog Timer.
 *  @param Timeout       Watchdog Timer timeout value.
 *  @return EFI_SUCCESS  Successfully.
 *  @return Other value  Something failed.
 */
EFI_STATUS
WatchdogTimerEnableAndDisable (
  IN UINTN  Timeout
  )
{
  UINT8 Flag = 0;
  EFI_STATUS Status = EFI_SUCCESS;

  // Set the Watchdog Timer
  Status = gtBS->SetWatchdogTimer (Timeout, 0x0000, 0x00, NULL);
  if ((Timeout == 0) && (EFI_ERROR(Status))) {
    SctPrint(L"Error: Failed to disable Watchdog timer - Status - %r\n", Status);
  }
  else if ((Timeout != 0) && EFI_ERROR(Status)) {
    SctPrint(L"Error: Failed to enable Watchdog timer - Status - %r\n", Status);
  }
  else {
    Flag = (Timeout == 0)? 1 : 0;
    if(Flag == 1) {
        SctPrint(L"Disabled Watchdog timer - Status - %r\n", Status);
    }
    else {
        SctPrint(L"Enabled Watchdog Timer - Status - %r\n", Status);
    }
  }

  return Status;
}

 /* GetAndSetFgtStatusVariable  - To read or wirte NVRAM variable.
 *  @param Type   GET_VARAIBLE/SET_VARIABLE - To read/write value from or to NVRAM.
 *  @param Value  Value to be retrieved/stored from or to NVRAM.
 *  @param Size   Size of the variable.
 *  @return EFI_SUCCESS  Successfully.
 *  @return Other value  Something failed.
 */
EFI_STATUS
GetAndSetFgtStatusVariable (
  IN UINT8    Type,
  IN UINT32  *Value,
  IN UINT32  *Size
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;

  if(Type == SET_VARIABLE) {
    Status = gtRT->SetVariable (
                  L"FgtAccessStatus",
                  &gFgtVariableGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  *Size,
                  (UINTN *)Value
                  );
  }
  else if(Type == GET_VARIABLE) {
    Status = gtRT->GetVariable (
                  L"FgtAccessStatus",
                  &gFgtVariableGuid,
                  NULL,
                  (UINTN *)Size,
                  (UINTN *)Value
                    );
  }

  if (EFI_ERROR (Status)) {
    if (Type == SET_VARIABLE) {
      SctPrint(L"Failed to write nvram fgt test status, Status - %r\n", Status);
    }
    else {
      SctPrint(L"Failed to read nvram fgt test status, Status - %r\n", Status);
    }
  }

  return Status;
}

 /* PeContextSave  - Save context data (LR, SP and ELR in case of unexpected exception).
 *  @param Sp   Stack Pointer.
 *  @param Elr  ELR register.
 *  @return     None.
 */
VOID
PeContextSave (
  IN UINT64  Sp,
  IN UINT64  Elr
  )
{
  gStackPointer = Sp;
  gExceptionRetAddr = Elr;
  gRetAddr = *(UINT64 *)(gStackPointer+8);

}

 /* PeUpdateElr  - Update the ELR to return from exception handler to a desired address.
 *  @param Context  A pointer to exception context structure.
 *  @param Offset   Address with which ELR should be updated.
 *  @return         None.
 */
VOID
PeUpdateElr (
  IN VOID    *Context,
  IN UINT64  Offset
  )
{
  ((EFI_SYSTEM_CONTEXT_AARCH64*)Context)->ELR = Offset;
}

 /* PeGetEsr  - Get the Exception syndrome from UEFI exception handler.
 *  @param Context   A pointer to exception context structure.
 *  @return          None.
 */
UINT64
PeGetEsr (
  IN VOID *Context
  )
{
  return ((EFI_SYSTEM_CONTEXT_AARCH64*)Context)->ESR;
}

 /* PeGetFar  - Get the FAR from UEFI exception handler.
 *  @param Context   A pointer to exception context structure.
 *  @return          None.
 */
UINT64
PeGetFar (
  IN VOID *Context
  )
{
  return ((EFI_SYSTEM_CONTEXT_AARCH64*)Context)->FAR;
}

 /* SynchrnousExceptionHandler  - Handler that triggers when sychronous exception triggers.
 *  @param InterruptType  Specifies which type of interrupt occurs.
 *  @param Context        A pointer to the interrupt context.
 *  @return               None
 */
VOID
SynchrnousExceptionHandler (
  IN UINT64  InterruptType,
  IN VOID    *Context
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  SctPrint(L"Unexpected exception of type %d occurred\n", InterruptType);
  SctPrint(L"FAR reported = 0x%llx\n", PeGetFar(Context));
  SctPrint(L"ESR reported = 0x%llx\n", PeGetEsr(Context));
  WatchdogTimerEnableAndDisable(0); //Disable Watchdog Timer
  gSyncIntReceived = 1;
  gFgtTestStatus = FGT_TEST_COMPLETED;
  Status = GetAndSetFgtStatusVariable(SET_VARIABLE, &gFgtTestStatus, &gFgtTestStatusSize);
  if (EFI_ERROR (Status))
    SctPrint(L"Error:%a:%d,  Status - %r\n", __FILE__, __LINE__, Status);

  PeUpdateElr(Context, gExceptionRetAddr);
}

 /* RegisterSynchronousExceptionHanler - Registers a synchronous exception handler.
 *  @param ExceptionType     EXCEPT_AARCH64_SYNCHRONOUS_EXCEPTIONS(0) or EXCEPT_AARCH64_SERROR(3).
 *  @param ExceptionHandler  Handler to handle Synchronous exceptions.
 *  @return EFI_SUCCESS      Successfully.
 *  @return Other value      Something failed.
 */
EFI_STATUS
RegisterSynchronousExceptionHandler (
  IN UINT32  ExceptionType,
  IN VOID    (*ExceptionHandler)(UINT64, VOID *)
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;

  // Get the CPU protocol that this driver requires.
  Status = gtBS->LocateProtocol (&gEfiCpuArchProtocolGuid, NULL, (VOID **)&gCpu);
  if(Status == EFI_SUCCESS) {
    // Unregister the default exception handler.
    Status = gCpu->RegisterInterruptHandler (gCpu, ExceptionType, NULL);
    if(Status == EFI_SUCCESS) {
      // Register to receive interrupts
      Status = gCpu->RegisterInterruptHandler (gCpu, ExceptionType, (EFI_CPU_INTERRUPT_HANDLER)ExceptionHandler);
      if(Status == EFI_SUCCESS) {
        SctPrint(L"Successfully registered ESR interrupt handler, Status - %r\n", Status);
      }
    }
  }

  return Status;
}

 /* CheckFeatFgtTrapAccess  - Entry point for CheckFeatFgtTrapAccess to test FGT register traps.
 *  @param StandardLib    A pointer to the Standard library.
 *  @return  None.
 */
VOID
CheckFeatFgtTrapAccess (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINTN WdtTimeout = 5;
  UINT64 HdfgrtrReg = 0;

  /*Start Watchdog timer*/
  WatchdogTimerEnableAndDisable(WdtTimeout);

  gFgtTestStatus = FGT_TEST_IS_IN_PROGRESS;
  Status = GetAndSetFgtStatusVariable(SET_VARIABLE, &gFgtTestStatus, &gFgtTestStatusSize);
  if (EFI_ERROR (Status)) {
    WatchdogTimerEnableAndDisable(0);
    StandardLib->RecordAssertion (
               StandardLib,
               EFI_TEST_ASSERTION_FAILED,
               gSbbrEfiTestGenericFailureGuid,
               L"Failed to write nvram fgt test status",
               L"%a:%d: Status - %r",
               __FILE__,
               (UINTN)__LINE__,
               Status
               );
    return;
  }

  gBranchToTest = &&ExceptionTaken;
  PeContextSave(AA64ReadSp(), (UINT64)gBranchToTest);
  Status = RegisterSynchronousExceptionHandler(EXCEPT_AARCH64_SYNCHRONOUS_EXCEPTIONS, SynchrnousExceptionHandler);
  if (EFI_ERROR (Status)) {
    WatchdogTimerEnableAndDisable(0);
    StandardLib->RecordAssertion (
               StandardLib,
               EFI_TEST_ASSERTION_FAILED,
               gSbbrEfiTestGenericFailureGuid,
               L"Failed to register esr interrupt handler",
               L"%a:%d: Status - %r",
               __FILE__,
               (UINTN)__LINE__,
               Status
               );
    return;
  }

  /* Check EL3 trap to access FEAT_FGT register */
  HdfgrtrReg = AA64ReadHdfgrtrEl2();
  while ((--gTimeout > 0) && (gSyncIntReceived == 0));
  if(gSyncIntReceived == 0) {
    WatchdogTimerEnableAndDisable(0);
    SctPrint(L"HDFGRTR_EL2 : 0x%llx\n", HdfgrtrReg);
    gFgtTestStatus = FGT_TEST_COMPLETED;
    Status = GetAndSetFgtStatusVariable(SET_VARIABLE, &gFgtTestStatus, &gFgtTestStatusSize);
    if (EFI_ERROR (Status)) {
      SctPrint(L"Failed to write nvram fgt test status, Status - %r\n", Status);
    }
    StandardLib->RecordAssertion (
               StandardLib,
               EFI_TEST_ASSERTION_PASSED,
               gSbbrEfiTestFgtFeatureTestAssertionGuid,
               L"Successfully read FGT features and also handled FGT trap successfully",
               L"%a:%d: Status - %r",
               __FILE__,
               (UINTN)__LINE__,
               Status
               );
  }
  else if(gSyncIntReceived == 1) {
    StandardLib->RecordAssertion (
               StandardLib,
               EFI_TEST_ASSERTION_FAILED,
               gSbbrEfiTestExceptionHandlerFailureGuid,
               L"Failed to read FGT register features but supports FGT trap handling",
               L"%a:%d: Status - %r",
               __FILE__,
               (UINTN)__LINE__,
               Status
               );
  }

ExceptionTaken:
  return;
}
