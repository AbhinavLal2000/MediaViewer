from PIL import Image

# Create a 128x64 grayscale image (8-bit pixels, black to white)
width, height = 128, 64
grayscale_image = Image.new('L', (width, height), color=128)  # middle gray background

# Save the image as BMP
grayscale_image.save("sample.bmp")

