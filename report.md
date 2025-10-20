# Mid Term Exam Report - Computer Graphics

---

## 1. Introduction

This assignment aims to analyze, identify, and implement an algorithm for basic line segment detection (horizontal, vertical, and diagonal) on a drawing canvas.

This report covers the findings regarding the ideal window size, the identified segment patterns, and the implementation details of the automated algorithm to detect and mark these segments.

---

## 2. Window Size and Segment Pattern Analysis

Based on the objectives to "analyze and find the correct window size" and "report all fitting window patterns," the following analysis was performed and served as the basis for the algorithm's implementation.

### 2.1. Ideal Window Size

The most ideal and efficient window size for identifying basic line segments is **3x3 pixels**.

**Reasoning:**
* A 3x3 size is the minimum required to determine a line passing through a central pixel `(x, y)`.
* This window allows for the inspection of all 8 neighbors surrounding a central pixel, which is sufficient to identify whether that pixel is part of a horizontal, vertical, or diagonal line.
* A smaller size (e.g., 1x1 or 2x2) would not provide enough contextual information. A larger size (e.g., 5x5) would significantly increase computational complexity and is unnecessary for simple segment detection.

### 2.2. Appropriate Window Patterns

Using a 3x3 window, four basic patterns were identified to represent a line segment. In the boolean matrix representations below, `1` represents a non-white pixel (part of a stroke), and `0` represents a white pixel.

The pixel `(x, y)` being tested is the center of the matrix, at index `(1, 1)`.

**1. Horizontal Pattern:**
[ 0 0 0 ] [ 1 1 1 ] [ 0 0 0 ]

* **Code Logic:** `window[1][0] && window[1][1] && window[1][2]`

**2. Vertical Pattern:**
[ 0 1 0 ] [ 0 1 0 ] [ 0 1 0 ]

* **Code Logic:** `window[0][1] && window[1][1] && window[2][1]`

**3. Diagonal Pattern (Left-to-Right):**
[ 1 0 0 ] [ 0 1 0 ] [ 0 0 1 ]

* **Code Logic:** `window[0][0] && window[1][1] && window[2][2]`

**4. Diagonal Pattern (Right-to-Left):**
[ 0 0 1 ] [ 0 1 0 ] [ 1 0 0 ]

* **Code Logic:** `window[0][2] && window[1][1] && window[2][0]`

The algorithm considers a pixel as a segment candidate if **any one** of these four patterns is met.

---

## 3. Detection Algorithm Implementation

An automated algorithm was developed within the `void DrawingCanvas::segmentDetection()` function based on the findings in Section 2.

The implementation process is as follows:

1.  **Image Capture:** The current canvas is captured as a `QPixmap` using `this->grab()`, then converted to a `QImage` to allow for pixel-by-pixel analysis.
2.  **Storage Initialization:** A `QVector<QPoint>` named `purpleRects` is prepared to store the `(x, y)` coordinates of all detected candidate pixels.
3.  **Pixel Iteration:** A nested loop iterates through each pixel of the image, starting from `(1, 1)` and ending at `(width-2, height-2)` to ensure the 3x3 window does not go out of the image bounds.
4.  **3x3 Window Extraction:** For each pixel `(x, y)`, a 3x3 boolean matrix (`bool window[3][3]`) is created. Each cell is populated with `true` if the corresponding pixel's color is *not* `Qt::white`, and `false` otherwise.
5.  **Pattern Matching:** The four boolean conditions (horizontal, vertical, diag1, diag2) described in Section 2.2 are checked.
6.  **Candidate Storage:** If any of the four conditions evaluate to `true`, the central pixel `(x, y)` is added to the `purpleRects` `QVector`.
7.  **Marking Candidates:** After all pixels have been scanned, a `QPainter` is used to draw on top of the original `QPixmap`.
    * The painter is configured with a solid purple brush (`Qt::magenta`) and no pen (`Qt::NoPen`).
    * It iterates through `purpleRects`, and for each `QPoint`, a 3x3 purple rectangle (`painter.drawRect(pt.x() - 1, pt.y() - 1, 3, 3)`) is drawn, centered on the candidate pixel.
8.  **Result Saving:** The modified `QPixmap` (now containing the purple markers) is saved to a file named `segment_detection_result.png`.
9.  **View Update:** The `update()` function is called to refresh the canvas display, making the purple rectangles visible to the user.

---

## 4. Results

The implemented algorithm successfully detected and marked pixels that are part of a straight-line segment. The output is saved in the `segment_detection_result.png` file.


The image shows the original strokes with an overlay of small purple rectangles, which mark every candidate pixel detected by the algorithm.

---

## 5. Appendix: AI Usage History

Below is the list of prompts used to build the logic in the `segmentDetection` function:

1.  "How to get pixel color from a QImage at specific (x, y) coordinates in Qt 6?"
2.  "How to check if a QColor object is equal to Qt::white in C++?"
3.  "Best way to store 3x3 pixel window data from QImage loop"
4.  "How to store a list of 2D coordinates in Qt?"
5.  "How to add a new QPoint to a QVector in Qt?"
6.  "How to draw shapes on an existing QPixmap in Qt 6?"
7.  "QPainter draw rectangle with purple fill and no outline"
8.  "How to save a QPixmap to a PNG file in Qt?"
9.  "How to force a QWidget to repaint after drawing with QPainter?"

Link:


https://gemini.google.com/app/b91bea09f8c80e11?hl=id