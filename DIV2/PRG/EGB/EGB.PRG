PROGRAM EGB;
BEGIN

   set_mode(m640x480);
   repeat

    FRAME;
   until(key(_esc));
   let_me_alone();
END

PROCESS renderMap()
BEGIN

END