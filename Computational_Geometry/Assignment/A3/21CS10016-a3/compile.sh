#!/bin/bash

# Source tex file
FILE="HW-3.tex"
BASENAME="${FILE%.tex}"

# Check if the file exists
if [ ! -f "$FILE" ]; then
  echo "Error: File '$FILE' does not exist."
  exit 1
fi

# Compile the .tex file (first pass)
echo "Compiling $FILE (first pass)..."
pdflatex -interaction=nonstopmode "$FILE"

if [ $? -ne 0 ]; then
  echo "First compilation failed. Check the .log file for details."
  exit 1
fi

echo "Running BibTeX..."
biber "$BASENAME"

if [ $? -ne 0 ]; then
  echo "BibTeX compilation failed. Check the .blg file for details."
  exit 1
fi

# Compile again to resolve references
echo "Compiling $FILE (second pass)..."
pdflatex -interaction=nonstopmode "$FILE"

if [ $? -ne 0 ]; then
  echo "Second compilation failed. Check the .log file for details."
  exit 1
fi

# Final compilation to fully resolve citations and cross-references
echo "Compiling $FILE (final pass)..."
pdflatex -interaction=nonstopmode "$FILE"

if [ $? -ne 0 ]; then
  echo "Final compilation failed. Check the .log file for details."
  exit 1
fi

# Clean up auxiliary files
echo "Cleaning up auxiliary files..."
rm -rf *.aux *.log *.out *.toc *.fdb_latexmk *.fls *.synctex.gz *.bbl *.blg *.bcf *.run.xml

echo "Done!"