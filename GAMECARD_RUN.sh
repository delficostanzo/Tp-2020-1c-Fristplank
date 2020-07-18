if test -f GameCard/cokemones/Metadata/Bitmap.bin; then
	rm GameCard/cokemones/Metadata/Bitmap.bin
	rm -r GameCard/cokemones/Blocks
	rm -r GameCard/cokemones/Files
fi

export LD_LIBRARY_PATH=/home/utnso/tp-2020-1c-Fritsplank/Library/Library/Debug
cd GameCard/Debug
./GameCard