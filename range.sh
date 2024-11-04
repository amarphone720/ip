#!/bin/bash

# Function to convert an IP address to a numeric value
ip_to_num() {
    local a b c d
    IFS=. read -r a b c d <<< "$1"
    echo $(( (a << 24) + (b << 16) + (c << 8) + d ))
}

# Function to convert a numeric value back to an IP address
num_to_ip() {
    local num=$1
    echo "$(( (num >> 24) & 255 )).$(( (num >> 16) & 255 )).$(( (num >> 8) & 255 )).$(( num & 255 ))"
}

# Check for correct number of arguments
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <lower_ip> <higher_ip>"
    exit 1
fi

lower_ip="$1"
higher_ip="$2"

# Convert IP addresses to numbers
lower_num=$(ip_to_num "$lower_ip")
higher_num=$(ip_to_num "$higher_ip")

# Check if the range is at least 1000
if [ $((higher_num - lower_num)) -lt 500 ]; then
    echo "The range must be at least 500."
    exit 1
fi

# Break the range into segments of 1000
while [ "$lower_num" -lt "$higher_num" ]; do
    next_num=$((lower_num + 1000))
    if [ "$next_num" -gt "$higher_num" ]; then
        next_num="$higher_num"
    fi

    # Convert back to IP and run the command
    lower_ip_segment=$(num_to_ip "$lower_num")
    next_ip_segment=$(num_to_ip "$next_num")
    
    echo "./ip22.sh $lower_ip_segment $next_ip_segment"
    
    # Update lower_num for the next iteration
    lower_num="$next_num"
done
