#!/bin/bash

# Source tex file
FILE="HW-5.tex"

# Check if the file exists
if [ ! -f "$FILE" ]; then
  echo "Error: File '$FILE' does not exist."
  exit 1
fi

# Compile the .tex file (twice to resolve references)
echo "Compiling $FILE (first pass)..."
pdflatex -interaction=nonstopmode "$FILE"

if [ $? -eq 0 ]; then
  echo "First compilation successful. Resolving references..."
  pdflatex -interaction=nonstopmode "$FILE"
else
  echo "First compilation failed. Check the .log file for details."
  exit 1
fi

# Check if the second compilation was successful
if [ $? -eq 0 ]; then
  echo "Second compilation successful!"
else
  echo "Second compilation failed. Check the .log file for details."
  exit 1
fi

# Clean up auxiliary files
echo "Cleaning up auxiliary files..."
rm -rf *.aux *.log *.out *.toc *.fdb_latexmk *.fls *.synctex.gz

echo "Done!"
