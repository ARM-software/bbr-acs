echo -off

for %i in 0 1 2 3 4 5 6 7 8 9 A B C D E F
  if exist FS%i:\EFI\BOOT\bbr\SCT then
    #
    # Found EFI SCT harness
    #
    FS%i:
    cd FS%i:\EFI\BOOT\bbr\SCT
    echo Press any key to stop the EFI SCT running
    stallforkey.efi 5
    if %lasterror% == 0 then
      goto Done
    endif
    for %j in 0 1 2 3 4 5 6 7 8 9 A B C D E F then
        if exists FS%j:\acs_results\ then
            if exists FS%j:\acs_results\sct_results then
                if exist FS%i:\EFI\BOOT\bbr\SCT\.passive.mode then
                    if exist FS%i:\EFI\BOOT\bbr\SCT\.verbose.mode then
                        Sct -c -p mnp -v
                    else
                        Sct -c -p mnp
                    endif
                    else
                    if exist FS%i:\EFI\BOOT\bbr\SCT\.verbose.mode then
                        Sct -c -v
                    else
                        Sct -c
                    endif
                    goto Done
                endif
            else
            FS%j:
            cd FS%j:\acs_results
            mkdir sct_results
            FS%i:
            cd FS%i:\EFI\BOOT\bbr\SCT
            Sct -s BBSR.seq      
            goto Done
            endif
        endif
    endfor
  endif
endfor

:Done

