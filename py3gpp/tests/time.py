import matplotlib.pyplot as plt

# File paths
files = [
    '/home/dima/Documents/PA2/Output/time_arm.txt',
    '/home/dima/Documents/PA2/Output/time_c.txt',
    '/home/dima/Documents/PA2/Output/time_cpp.txt',
    '/home/dima/Documents/PA2/Output/time_python.txt'
]

# Initialize lists to hold the times in milliseconds and labels
times_ms = []
labels = ['ArmRAN', 'srsRAN_4G', 'srsRAN_Project', 'py3gpp']

# Read time data from each file and convert to milliseconds
for file in files:
    with open(file, 'r') as f:
        line = f.readline().strip()
        # Extract the time value from the line
        time_value_seconds = float(line.split(': ')[1].split(' ')[0])
        # Convert seconds to milliseconds
        time_value_ms = time_value_seconds * 1000
        times_ms.append(time_value_ms)

# Plotting
plt.figure(figsize=(10, 6))
bars = plt.bar(labels, times_ms, color=['blue', 'green', 'red', 'purple'])
plt.xlabel('Algorithm')
plt.ylabel('Mean Time Taken (milliseconds)')
plt.title('Mean Time Taken for Encoding by Different Algorithms')
plt.yscale('log')  # Optional: use logarithmic scale if time values span several orders of magnitude

# Adding text annotations above each bar
for bar in bars:
    yval = bar.get_height()
    # Format the value to 4 decimal places
    plt.text(bar.get_x() + bar.get_width()/2, yval, f'{yval:.4f} ms', 
             ha='center', va='bottom')

plt.grid(True, which='both', linestyle='--', linewidth=0.5)
plt.show()

