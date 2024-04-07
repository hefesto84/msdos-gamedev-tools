                   TCHRT V3 Manual Corrections and Additions
                   -----------------------------------------


The file DISTRIB.TXT contains a description of all files in this archive.


t_report & t_bios_report
------------------------
An important caveat is not mentioned in the manual for these two functions:
They may not be called after t_stop or t_bios_stop have been called.  
t_report issues a warning if called after t_stop - t_bios_report does not.
Any active BIOS timers are suspended during t_bios_report, so the report
output will not affect CRT, disk, or printer interrupt timing.



