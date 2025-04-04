# Train Booking System

**University Project for IMS511**

## **Description**

`Train Booking System` is a comprehensive C++14-based train reservation system developed as a final project for the **IMS511**. The application facilitates the management of train routes, schedules, carriages, seat reservations, discount schemes, and user accounts. It is designed to simulate real-world train booking scenarios, ensuring efficient handling of reservations and user interactions.

## **Features**

- **User Management**
  - Register new users with validation.
  - Secure login system for existing users.

- **Route Management**
  - Create and manage multiple train routes.
  - Define starting and ending stations with schedules and base pricing.

- **Train and Carriage Management**
  - Add, view, and remove trains (`KeteApi`) and their carriages (`Gerabak`).
  - Each carriage comprises rows and columns of seats (`TempatDuduk`) with reservation capabilities.

- **Seat Reservation**
  - Book, view, and cancel seat reservations for specific months.
  - Visual representation of seat availability within carriages.

- **Discount Management**
  - Apply various discount categories based on user profiles and payment methods.
  - Seasonal discounts with active periods.

- **Booking and Receipts**
  - Generate unique booking IDs and receipts for each reservation.
  - Manage booking history and receipt storage.

## **Technologies Used**

- **Programming Language:** C++14
- **IDE:** Visual Studio
- **Version Control:** Git & GitHub

## **Installation**

1. **Clone the Repository:**
   git clone https://github.com/yourusername/protoTrain2.git

2. **Open the Project in Visual Studio:**

   - Launch **Visual Studio**.
   - Select **File > Open > Project/Solution**.
   - Navigate to the cloned `protoTrain2` directory and open the `protoTrain2.vcxproj` file.

3. **Build the Project:**

   - In Visual Studio, navigate to **Build > Build Solution** or press `Ctrl + Shift + B`.
   - Ensure there are no compilation errors.

## **Usage**

1. **Run the Application:**

   - Press `F5` or navigate to **Debug > Start Debugging** in Visual Studio.

2. **Interact with the System:**

   - **Register:** Create a new user account with valid email and age.
   - **Login:** Access the system using registered credentials.
   - **Manage Routes:** View available routes or add new ones.
   - **Manage Trains:** Add or remove trains and their carriages.
   - **Make Reservations:** Select routes, trains, and reserve specific seats.
   - **Apply Discounts:** Utilize available discount schemes during booking.
   - **View Bookings:** Check existing reservations and generate receipts.
   - **Cancel Reservations:** Remove bookings and free up seats.

## **Project Structure**

- **Classes:**
  - `Pengguna`: Handles user information and authentication.
  - `Laluan`: Manages train routes and schedules.
  - `KeteApi`: Represents trains and their associated carriages.
  - `Gerabak`: Manages individual carriages and seat layouts.
  - `TempatDuduk`: Represents individual seats with reservation status.
  - `PengurusanDiskaun`: Manages discount categories and calculations.
  - `Penempahan`: Handles booking operations and receipt generation.

## Limitations

- **Language Use:**
  - The codebase extensively uses Malay language for variable names, class names, and print statements. This may cause confusion for non-Malay speakers and could hinder collaboration with developers who are not proficient in Malay.
  - Comments and documentation are also primarily in Malay, which may affect the ease of understanding and maintaining the code.

- **Internationalization:**
  - The application currently lacks support for multiple languages or localization, limiting its usability to Malay-speaking users.

- **Error Handling:**
  - Some areas of the code may lack comprehensive error handling and validation, which could lead to unexpected crashes or undefined behaviors under certain conditions.
  - Input validation could be enhanced to ensure robustness.

- **Scalability:**
  - The system is designed for a limited set of use cases and may not scale efficiently with a large number of users, trains, or bookings without optimization.

- **User Interface:**
  - The application operates via a console-based interface, which may not provide the best user experience compared to graphical interfaces.
  - Navigation and usability could be improved to make the system more user-friendly.

- **Data Persistence:**
  - Current implementation lacks a persistent database, relying on in-memory data structures. This means that all data is lost when the application terminates.
  - There is no implementation for saving and loading data from files or databases.

- **Security:**
  - Passwords are stored in plain text within the application's memory, posing a security risk.
  - There is no encryption or secure authentication mechanisms implemented.

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request for any enhancements or bug fixes.

## License

This project is licensed under the [MIT License](LICENSE).

## **Acknowledgements**

This project was developed as part of the final requirements for the university course **IMS511**. Special thanks to the instructors and peers who provided guidance and support throughout the development process.

## **
---


