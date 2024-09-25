local nybble_to_hamming = { [0]=0x00, 0xD2, 0x55, 0x87, 0x99, 0x4B, 0xCC, 0x1E, 0xE1, 0x33, 0xB4, 0x66, 0x78, 0xAA, 0x2D, 0xFF }

local rev = {}

local masks = {}
for a = 0, 7 do
    for b = 0, 7 do
        if a ~= b then
            table.insert(masks, (1<<a)|(1<<b))
        end
    end
end

for i = 0, 15 do
    local code = nybble_to_hamming[i]
    rev[code] = {i, 0}
    -- 1-bit errors:
    for j = 0, 7 do
        rev[code ~ (1<<j)] = {i, 1}
    end
    -- 2-bit errors:
    for _, mask in pairs(masks) do
        -- TODO: balance the distribution of selections for 2-bit errors?
        rev[code ~ mask] = {i, 2}
    end
end

io.write("const byte hamming_to_nybble[256] = {")
for i = 0, 255 do
    if i%16 == 0 then
        io.write("\n\t")
    end
    io.write(string.format( "0x%02x, ", rev[i][1] | (rev[i][2] << 4) ))
end
io.write("\n};")
