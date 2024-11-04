#!/bin/bash

# Usage: ./generate_ips.sh <start_ip> <end_ip>
# Example: ./generate_ips.sh 192.168.1.1 192.168.1.10

# Convert IP to an integer
ip_to_int() {
    local IFS=.
    read -r i1 i2 i3 i4 <<< "$1"
    echo $(( (i1 << 24) + (i2 << 16) + (i3 << 8) + i4 ))
}

# Convert integer back to IP
int_to_ip() {
    echo "$(( ($1 >> 24) & 255 )).$(( ($1 >> 16) & 255 )).$(( ($1 >> 8) & 255 )).$(( $1 & 255 ))"
}

# Start and end IPs
start_ip="$1"
end_ip="$2"

# Convert IPs to integers
start_int=$(ip_to_int "$start_ip")
end_int=$(ip_to_int "$end_ip")

# Initialize an array to store IPs
ips=()

# Batch size
batch_size=400

# Generate the IP range and process in batches
for ((i=start_int; i<=end_int; i++)); do
    ip=$(int_to_ip "$i")
    ips+=("$ip")

    # If batch size is reached, process the batch
    if (( ${#ips[@]} == batch_size )); then
        for ip in "${ips[@]}"; do
            ./get "$ip" &
        done
        wait  # Wait for the batch to finish
        ips=()  # Reset the batch array
    fi
done

# Process any remaining IPs in the final batch
if (( ${#ips[@]} > 0 )); then
    for ip in "${ips[@]}"; do
        ./get "$ip" &
    done
    wait  # Wait for the final batch to finish
fi
