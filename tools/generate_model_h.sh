#!/bin/sh
DEVICE_FILE=LOR_DeviceFile.txt
HEADER_FILE=include/model.h

# Download and overwrite $DEVICE_FILE using quiet mode
echo "Downloading $DEVICE_FILE"
wget http://www1.lightorama.com/downloads/LOR_DeviceFile.txt -q -O $DEVICE_FILE

# Generate a sha1sum of the downloaded file
# This is stored in the generated header file for validation purposes
DEVICE_FILE_SUM=$(sha1sum $DEVICE_FILE | cut -d " " -f 1)

# Create new header file
echo "Generating $HEADER_FILE"
rm -f $HEADER_FILE

BUILD_DATE=$(date)
BUILD_AUTHOR="git: $(git config user.name) <$(git config user.email)>"
BUILD_FILE=$(basename "$0")

# Append LICENSE file as a comment
echo "// AUTOMATICALLY GENERATED, DO NOT MODIFY THIS FILE!
// $DEVICE_FILE sha1sum: $DEVICE_FILE_SUM
// Built $BUILD_DATE
// $BUILD_AUTHOR
// $BUILD_FILE

/*" >> $HEADER_FILE
cat LICENSE | while read LINE
do
  echo " * $LINE" >> $HEADER_FILE
done
echo " */
#ifndef LIGHTORAMA_MODEL_H
#define LIGHTORAMA_MODEL_H
" >> $HEADER_FILE

# Use tail -n+2 to skip the 1st line $LOR_DEVICEFILE containing column names
tail -n+2 $DEVICE_FILE | while read LINE || [ -n "$LINE" ]
do
	MODEL_NUMBER=$(echo "$LINE" | cut -d, -f 1 | sed 's/^0*//')	# Split on "," get the 1st column, remove any leading zeroes from $MODEL_NUMBER
	
	MODEL_NUMBER_HEX=$(printf '%02x' $MODEL_NUMBER)				# Generate hexadecimal equivalent of $MODEL_NUMBER

	MODEL_TYPE=$(echo "$LINE" | cut -d, -f 2 | xargs)			# Split on "," get the 2nd column, trim leading & trailing whitespace using xargs
	MODEL_TYPE=${MODEL_TYPE//.?}								# Strip the version format (".?") leaving only the major version
	MODEL_TYPE=${MODEL_TYPE//-/ }								# Replace hyphens with spaces
	MODEL_TYPE=$(echo "$MODEL_TYPE" | tr -s ' ')				# Remove repeating spaces
	MODEL_TYPE=${MODEL_TYPE//VER /V}							# Reformat "VER n" to "VN"
	MODEL_TYPE=${MODEL_TYPE// /_}								# Replace spaces with underscores
	MODEL_TYPE="LOR_MODEL_${MODEL_TYPE}_${MODEL_NUMBER_HEX}"	# Prepend "LOR_MODEL_" prefix, append MODEL_NUMBER_HEX to ensure unique name
	MODEL_TYPE=$(echo "$MODEL_TYPE" | tr a-z A-Z)				# Uppercase the string

	# Add hexadecimal MODEL_NUMBER via comment for human referencing
	# Use /**/ to avoid preprocessor issues, see https://stackoverflow.com/questions/1510869/does-the-c-preprocessor-strip-comments-or-expand-macros-first
	echo "#define $MODEL_TYPE $MODEL_NUMBER /* 0x$MODEL_NUMBER_HEX */" >> $HEADER_FILE
done

# End header definition
echo "
#endif //LIGHTORAMA_MODEL_H" >> $HEADER_FILE

# Remove $DEVICE_FILE download
rm -f $DEVICE_FILE
