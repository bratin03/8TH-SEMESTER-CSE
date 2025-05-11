#!/bin/bash

# Default options
show_output=false
strict_error=false

# Process command-line options
while getopts "ie" opt; do
  case $opt in
    i)
      show_output=true
      ;;
    e)
      strict_error=true
      ;;
    *)
      echo "Usage: $0 [-i] [-e]"
      exit 1
      ;;
  esac
done

# Hardcoded TeX file
FILE="A1.tex"

# Check if the file exists
if [ ! -f "$FILE" ]; then
  echo "Error: File '$FILE' does not exist."
  exit 1
fi

# Function to run pdflatex with optional output display
run_pdflatex() {
  local pass_label="$1"
  echo "Compiling $FILE ($pass_label pass)..."
  
  if $show_output; then
    pdflatex -interaction=nonstopmode "$FILE"
  else
    pdflatex -interaction=nonstopmode "$FILE" > /dev/null 2>&1
  fi
  
  return $?
}

# First compilation pass
run_pdflatex "first"
result=$?
if [ $result -ne 0 ]; then
  echo "First compilation failed. Check the .log file for details."
  if $strict_error; then
    echo "Stopping due to error (-e flag provided)."
    exit 1
  fi
else
  echo "First compilation successful. Resolving references..."
fi

# Second compilation pass
run_pdflatex "second"
result=$?
if [ $result -ne 0 ]; then
  echo "Second compilation failed. Check the .log file for details."
  if $strict_error; then
    echo "Stopping due to error (-e flag provided)."
    exit 1
  fi
else
  echo "Second compilation successful!"
fi

# Clean up auxiliary files
echo "Cleaning up auxiliary files..."
rm -rf *.aux *.log *.out *.toc *.fdb_latexmk *.fls *.synctex.gz

echo "Done!"
