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

    cv2.drawContours(img, contours, -1, (255,0,0), thickness=2)
    print("showing contours")
    print(contours)
    cv2.imshow('contours', img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    print('done')

    # Iterate over contours to find moments and compute theta
    for contour in contours:

        moments = cv2.moments(contour)
        I = moments['m00']
        if I == 0:
            print("No moments found")
            return
        m10 = moments['m10']
        m01 = moments['m01']
        cx = m10 / I
        cy = m01 / I
        m11 = moments['m11']

        #print(f"Center: ({cx}, {cy})")

        ax = moments['m20']
        ay = moments['m02']

        theta = 0.5 * np.arctan2((2 * ((I * m11) - (m10 * m01))), (((I * ax) - np.power(m10, 2)) - ((I * ay) - np.power(m01, 2))))

        #print(f"Theta: {np.degrees(theta)}")

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
    ref_angle = 5

    #blob with the target orientation
    ref_blob = generate_blob(size, ref_angle)

    output = []
    for delta in range(0, 180 - ref_angle, 2): #step size of 2
        print(f"--Delta: {delta}")
        current_angle = ref_angle + delta
        #10 trials
        for i in range(num_trials):
            # Generate a blob with the target orientation
            blob = generate_blob(size, current_angle)
            #cv2.imshow('blob', generated_blob)
            #cv2.waitKey(0)
            #cv2.destroyAllWindows()

            ref_orientation = detect_orientation(ref_blob)
            blob_orientation = detect_orientation(blob)

            print(f"Ref: {ref_orientation}, Blob: {blob_orientation}")

            if blob_orientation == None and ref_orientation == None:
                continue

            if ref_orientation == None:
                continue

            if blob_orientation == None:
                correct_response += 1
                continue


            if abs(ref_orientation - ref_angle) < abs(blob_orientation - ref_angle):
                correct_responses += 1

        output.append(correct_responses / num_trials)

    return output 



# Define parameters
num_trials = 10 # Number of trials to perform for each delta
blob_size = 100  # Size of each blob (width and height in pixels)

# Measure orientation discrimination threshold
result = measure_orientation_discrimination_threshold(num_trials, blob_size)
#print(f"Orientation discrimination accuracy: {accuracy * 100:.2f}%")
print(result)
