#!/bin/sh

SESSION=$USER

tmux -2 new-session -d -s $SESSION

tmux split-window -v
tmux select-pane -t 0
tmux send-keys "sshpass -p vtclab ssh  -X vtclab@192.168.1.101" C-m
tmux send-keys "cd John/SDR/SDR/Flowgraphs/webinterface" C-m
tmux select-pane -t 1
tmux send-keys "sshpass -p vtclab ssh -X vtclab@192.168.1.101" C-m
tmux send-keys "cd John/SDR/SDR/Flowgraphs/webinterface" C-m
tmux new-window
tmux split-window -v
tmux select-pane -t 0
tmux send-keys "cd ~/John/SDR/SDR/Flowgraphs/webinterface" C-m
tmux select-pane -t 1
tmux send-keys "cd ~/John/SDR/SDR/Flowgraphs/webinterface" C-m

tmux -2 attach-session -t $SESSION
