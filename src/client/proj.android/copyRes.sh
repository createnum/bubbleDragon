for file in "$1"/*
do
if [ -d "$file" ]; then
    cp -rf "$file" "$2"
fi

if [ -f "$file" ]; then
    cp "$file" "$2"
fi
done
