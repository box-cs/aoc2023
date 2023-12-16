#!/usr/bin/bash
if [[ "$1" = "-h" ]] || [[ "$1" = "-help" ]]; then
    echo "Usage: ./run.sh <dd> to build and run the program for the given day in this year"
    echo "Usage: ./run.sh <dd-yyyy> to build and run the program for the given date"
    exit
fi

if [[ "$1" == "" ]]; then
    current_date="$(date -d '+1 day' '+%d-%m-%Y')"
    day="$(echo $current_date | awk -F - '{print $1}')"
    year="$(echo $current_date | awk -F - '{print $3}')"
else
    day="$(echo $1 | awk -F - '{print $1}')"
    year="$(echo $1 | awk -F - '{print $2}')"
    current_date="$day-12-$year"
    if [[ -z $year ]]; then
        year="$(date '+%Y')"
    fi
fi
cmake -B ./build -DTARGET_NAME=$day-12-$year
cd build

echo "Building $day-12-$year:"
cmake --build . --target $day-12-$year &&
echo "Running $day-12-$year:" &&
./$day-12-$year
