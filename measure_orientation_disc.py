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

def detect_orientation(image):
    contours, _ = cv2.findContours(image, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    img = image.copy()

    #cv2.drawContours(img, contours, -1, (255,0,0), thickness=2)
    #print("showing contours")
    #print(contours)
    #cv2.imshow('contours', img)
    #cv2.waitKey(0)
    #cv2.destroyAllWindows()
    #print('done')

    # Iterate over contours to find moments and compute theta
    for contour in contours:

        moments = cv2.moments(contour)
        I = moments['m00']
        m10 = moments['m10']
        m01 = moments['m01']
        cx = m10 / I
        cy = m01 / I
        m11 = moments['m11']

        print(f"Center: ({cx}, {cy})")

        ax = moments['m20']
        ay = moments['m02']

        theta = 0.5 * np.arctan2((2 * ((I * m11) - (m10 * m01))), (((I * ax) - np.power(m10, 2)) - ((I * ay) - np.power(m01, 2))))

        print(f"Theta: {np.degrees(theta)}")

        cv2.line(image, (int(cx) - 15, int(cy) + 15), (int(cx) + 15, int(cy) - 15), (0, 255, 255), thickness=2)
        cv2.line(image, (int(cx) + 15, int(cy) + 15), (int(cx) - 15, int(cy) - 15), (0, 255, 255), thickness=2)

        cv2.line(image, (int(cx) + 50, int(cy) + int(theta * 50)), (int(cx) - 50, int(cy) - int(theta * 50)), (255, 255, 0), thickness=2)

       # cv2.imshow('blob contours', image)
       # cv2.waitKey(0)
       # cv2.destroyAllWindows()

    return theta


def measure_orientation_discrimination_threshold(num_trials, size):
    correct_responses = 0

    # 5deg
    target_orientation = 5

    #blob with the target orientation
    target_blob = generate_blob(size, target_orientation)

    current_angle = target_orientation
    for _ in range(num_trials):
        # Generate a blob with the target orientation
        generated_blob = generate_blob(size, current_angle)
        #cv2.imshow('blob', generated_blob)
        #cv2.waitKey(0)
        #cv2.destroyAllWindows()

        # Your code to process the target_blob and measure the orientation
        # discrimination accuracy goes here. This could involve using
        # computer vision techniques like edge detection, contour analysis,
        # or machine learning models trained to detect orientations.

        detected_orientation = detect_orientation(generated_blob)

        # For demonstration purposes, let's assume the system correctly
        # identifies the orientation if it is within a tolerance range of 10 degrees.
        tolerance = 5
        #detected_orientation = np.random.randint(target_orientation - tolerance, target_orientation + tolerance + 1)
        
        # Check if the detected orientation is within the tolerance range
        if target_orientation - tolerance <= detected_orientation <= target_orientation + tolerance:
            correct_responses += 1

        current_angle += 5

    accuracy = correct_responses / num_trials
    return accuracy



# Define parameters
num_trials = 1000  # Number of trials to perform
blob_size = 100  # Size of each blob (width and height in pixels)

# Measure orientation discrimination threshold
accuracy = measure_orientation_discrimination_threshold(num_trials, blob_size)
print(f"Orientation discrimination accuracy: {accuracy * 100:.2f}%")
