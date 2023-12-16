#!/bin/bash
# .env contains $session
source .env

if [[ "$1" == "-h" ]]; then
    echo "Usage: <day-year> in format dd-yyyy"
    elif [[ "$1" == "-h" ]]; then
    current_date="$(date -d '+1 day' '+%d-%m-%Y')"
    day="$(echo $current_date | awk -F - '{print $1}')"
    year="$(echo $current_date | awk -F - '{print $3}')"
else
    day="$(echo $1 | awk -F - '{print $1}')"
    year="$(echo $1 | awk -F - '{print $2}')"
    current_date="$day-12-$year"
fi

day=$(expr $day + 0) # convert to int

if [[ $day ]] && [[ ${#year} == 4 ]]; then
    echo "Initializing files for $day-12-$year"
    mkdir "$current_date" && \
    curl https://adventofcode.com/$year/day/$day/input \
    -H $"Cookie: session=${session}" \
    >> $current_date/input.txt
    
    cp template.cpp "$current_date.cpp"
    mv "$current_date.cpp" $current_date
fi
