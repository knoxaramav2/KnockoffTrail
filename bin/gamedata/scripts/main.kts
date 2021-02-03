#main.kts always runs first

#constants, always start with '.'
.homescript 'home.kts'
.continuescript 'continue_game.kts'
.newgamescript 'new_game.kts'
.helpscript 'help.kts'

#execute home page script
:HOME
run .homescript

#switch
mov tri si
sjmp 0 NEW_GAME
sjmp 1 CONTINUE_GAME
sjmp 2 HELP
sjmp 3 QUIT 
sdef QUIT

:NEW_GAME
run .newgamescript
jmp MAIN

:CONTINUE_GAME
run .continuescript
jmp MAIN

:HELP
run .helpscript
jmp HOME

:MAIN

:QUIT
mov 0 tri
exit