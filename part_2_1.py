import cv2
import numpy as np

def generate_blob(size, orientation):
    # Create a blank image
    img = np.zeros((size, size), dtype=np.uint8)

    # Calculate the center of the image
    center = (size // 2, size // 2)

    # Draw an ellipse (blob) with the specified orientation
    axes = (int(size * 0.8 / 2), int(size * 0.2 / 2))  # Major and minor axes lengths
    cv2.ellipse(img, center, axes, orientation, 0, 360, 255, -1)

    return img

def display_blobs(num_blobs, size):
    # Create a black canvas to display blobs
    canvas = np.zeros((size * num_blobs, size), dtype=np.uint8)

    # Generate blobs with varying orientations and display them
    for i in range(num_blobs):
        orientation = i * (360 / num_blobs)  # Vary orientation for each blob
        blob = generate_blob(size, orientation)
        canvas[i * size : (i + 1) * size, :] = blob

    # Display the canvas with blobs
    cv2.imshow('Blobs with Varying Orientations', canvas)
    cv2.imwrite('result.png', canvas)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

# Define parameters
num_blobs = 12  # Number of blobs to display
blob_size = 100  # Size of each blob (width and height in pixels)

# Display blobs
display_blobs(num_blobs, blob_size)
