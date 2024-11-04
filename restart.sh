#!/bin/bash

# Specify the path to the text file containing the commands
input_file="range.txt"  # Replace with the actual path to your text file

while true; do
    # Read each line from the input file
    while IFS= read -r command; do
        # Start the command in the background
        echo "Starting command: $command"
        eval "$command" &  # Use eval to run the command as a background process
        pid=$!  # Get the process ID of the last background command

        # Wait for 60 seconds
        sleep 30

        # Kill the command after 60 seconds
        if ps -p $pid > /dev/null; then
            echo "Killing command: $command"
            kill $pid
        else
            echo "Command already exited: $command"
        fi

    done < "$input_file"  # Input file for reading

    # Optional: Add a sleep if you want to repeat the whole process after one complete pass
    # sleep 10
done
