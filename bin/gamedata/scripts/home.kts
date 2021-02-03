push_display_frame #adds frame for controlling new UI control

push_title 'Knockoff Trail'
push_option 'New Journey' 0 #add option for current UI control
push_option 'Continue' 1
push_option 'Help' 2
push_option 'Quit' 3

mov 42 brdr #'*'

wait_option #wait until option is selected, argument 2 set to opti (option int) register

mov opti tri #move opti result to tri (typical return integer) register

pop_display_frame #delete frame, free memory