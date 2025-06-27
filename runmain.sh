
#!/bin/bash

echo "Deleting main..."
make clean

echo "Making main..."
make

echo "Running main..."
./main 
