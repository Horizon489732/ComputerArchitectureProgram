from Ziv_Lempel import Ziv_Lempel
from PIL import Image

# converts pixels to bits
def convert_pixel_to_bits(pixel_value):
    i = 0
    pixel_binary = ""
    while i < len(pixel_value):
        pixel = pixel_value[i]
        binary_val = bin(pixel)[2:]

        if len(binary_val) < 8:
            binary_val = ("0" * (8 - len(binary_val))) + binary_val #Add padding till 8
        pixel_binary += binary_val
        i += 1
    return pixel_binary

# converts from RGB numbers to bits
def convert_RGB_to_bits(pixels, width, height):
    pixel_binary = "10"
    curr_height = 0

    # loop through the entire picture
    while curr_height < height:
        curr_width = 0
        while curr_width < width:
            pixel_value = pixels[curr_width, curr_height]
            # convert pixels to binary
            pixel_binary += convert_pixel_to_bits(pixel_value)
            curr_width += 1
        curr_height += 1
    return pixel_binary

# AI helped with this part
def convert_bits_to_RGB(bits):
    r = int(bits[0:8], 2)
    g = int(bits[8:16], 2)
    b = int(bits[16:24], 2)
    return r, g, b

# Main compression function
def compress_bmp_image(input_path, output_path):
    # Load image
    img = Image.open(input_path)
    pixels = img.load()
    width, height = img.size
    
    print(f"Image size: {width}x{height} pixels")
    print(f"Total pixels: {width * height}")
    
    # Convert pixels to bits starting with "10"
    binary_string = convert_RGB_to_bits(pixels, width, height) 
    print(f"Original length: {len(binary_string)} bits")
    
    # Ziv-Lempel algorithm
    zl = Ziv_Lempel(binary_string)
    zl.get_unique_substrings()
    zl.translate_substrings()
    encoded_message = zl.get_encoded_message()
    print(f"Compressed length: {len(encoded_message)} bits")
    
    if len(encoded_message) % 24 != 0:
        padding = 24 - (len(encoded_message) % 24)
        encoded_message += "0" * padding

    # Convert encoded message back to RGB and create new image
    new_img = Image.new('RGB', (width, height))
    new_pixels = new_img.load()
    
    bit_index = 0
    pixels_filled = 0
    
    curr_height = 0
    while curr_height < height:
        curr_width = 0
        while curr_width < width:
            if bit_index + 24 <= len(encoded_message):
                rgb_binary = encoded_message[bit_index:bit_index + 24]
                r, g, b = convert_bits_to_RGB(rgb_binary)
                new_pixels[curr_width, curr_height] = (r, g, b)
                bit_index += 24
                pixels_filled += 1
            else:
                # Not enough bits left, fill with black
                new_pixels[curr_width, curr_height] = (0, 0, 0)
            curr_width += 1
        curr_height += 1
    
    print(f"Pixels filled: {pixels_filled} out of {width * height}")
    
    new_img.save(output_path)
    print(f"Compressed image saved to: {output_path}")

if __name__ == "__main__":
    compress_bmp_image(input_path="images/example.bmp", output_path="output.bmp")