#!/bin/sh

# Requires sshpasss
# sudo apt-get install sshpass

SESSION=$USER

# Function to connect to all sessins
# format should be:
#		connect user pass ip
connect(){
	tmux new-window
	#tmux split-window -v

	#tmux select-pane -t 0

	ssh="sshpass -p $2 ssh -X $1@192.168.1.101"
	tmux send-keys "$ssh" C-m
	
	tmux send-keys "cd SDR/SDR/Flowgraphs" C-m



}

user='vtclab'
pass='vtclab'

# Start Session
tmux -2 new-session -d -s $SESSION

connect $user $pass

# Connect to setup session
tmux -2 attach-session -t $SESSION


# IP List
# 192.168.1.100
# 192.168.1.101
# 192.168.1.102
# 192.168.1.106
# 192.168.1.107
# 192.168.1.109
