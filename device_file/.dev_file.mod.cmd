savedcmd_/home/nurbol/Desktop/practice_kernel/device_file/dev_file.mod := printf '%s\n'   dev_file.o | awk '!x[$$0]++ { print("/home/nurbol/Desktop/practice_kernel/device_file/"$$0) }' > /home/nurbol/Desktop/practice_kernel/device_file/dev_file.mod