import java.io.*;

// Get the week day for a given date between 1900 and 2100
public class week {

    public static void main(String[] args) {
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
        System.out.print("Enter a date (dd.mm.yyyy): ");
        try {
            String date = reader.readLine();
            int dayOfWeek = getDayOfWeek(date);
            System.out.println("The day of the week is: " + DayMapping(dayOfWeek));
        } catch (IOException e) {
            System.out.println("Error reading input: " + e.getMessage());
        }
    }

    public static int[] parseDate(String date) {
        String[] parts = date.split("\\.");
        int[] result = new int[3];
        result[0] = Integer.parseInt(parts[0]); // day
        result[1] = Integer.parseInt(parts[1]); // month
        result[2] = Integer.parseInt(parts[2]); // year
        return result;
    }

    public static int isLeapYear(int year) {
        if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0) {
            return 1; // leap year
        } else {
            return 0; // not a leap year
        }
    }

    public static int getDaysInMonth(int month) {
        switch (month) {
            case 1: case 3: case 5: case 7: case 8: case 10: case 12:
                return 31;
            case 4: case 6: case 9: case 11:
                return 30;
            case 2:
                return 28;
            default:
                return -1; // invalid month
        }
    }

    public static String DayMapping(int dayOfWeek) {
        switch (dayOfWeek) {
            case 0: return "Sunday";
            case 1: return "Monday";
            case 2: return "Tuesday";
            case 3: return "Wednesday";
            case 4: return "Thursday";
            case 5: return "Friday";
            case 6: return "Saturday";
            default: return "Invalid day";
        }
    }

    public static int getDayOfWeek(String date) {
        int[] dateParts = parseDate(date);
        int day = dateParts[0];
        int month = dateParts[1];
        int year = dateParts[2];

        // count all leap years since 1900

        int leapYears = 0;
        for (int i = 1900; i < year; i++) {
            leapYears += isLeapYear(i);
        }

        if (month > 2) {
            leapYears += isLeapYear(year);
        }

        int months = 0;
        for (int i = 1; i < month; i++) {
            months += getDaysInMonth(i);
        }

        int days = (year - 1900) * 365 + day + months + leapYears;
        int dayOfWeek = (days % 7 + 7) % 7; // ensure positive
        return dayOfWeek;
    }
}