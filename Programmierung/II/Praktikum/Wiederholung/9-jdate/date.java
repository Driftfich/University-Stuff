import java.io.*;
import java.lang.*;

public class date {
    public static void main(String argv[]) {
        BufferedReader reader = new BufferedReader( new InputStreamReader(System.in));
        System.out.print("Enter 'break' to finish");
        try {
            while (true) {
                System.out.println("Enter a date (dd.mm.yy): ");
                String date = reader.readLine();
                if (date.matches(".*break.*")) break;
                int dayOfWeek = getDayOfWeek(date);
                System.out.println("Day of the week is: " + DayMapping(dayOfWeek));
            }
        }
        catch (IOException e) {System.out.println("Error reading input");}
    }

    public static int isLeapYear(int year) {
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) return 1;
        return 0;
    }

    public static int DayPerMonth(int month) {
        if (month < 0 || month >= 12) {throw new RuntimeException("No valid month given");}
        if (month == 1) return 28;
        else if ((month % 2 == 1 && month < 7) || (month % 2 == 0 && month >= 7)) return 30;
        else return 31;
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

    public static int[] parseDate(String date) {
        String[] sparts = date.split("\\.");
        int[] iparts = new int[3];
        iparts[0] = Integer.parseInt(sparts[0]);
        iparts[1] = Integer.parseInt(sparts[1]);
        iparts[2] = Integer.parseInt(sparts[2]);
        return iparts;
    }

    public static int getDayOfWeek(String date) {
        int[] dateParts = parseDate(date);
        int day = dateParts[0];
        int month = dateParts[1] - 1;
        int year = dateParts[2];

        int leapyears = 0;
        for (int i=1900; i<year; i++) {
            leapyears += isLeapYear(i);
        }

        if (month > 1) leapyears += isLeapYear(year);

        int predays = 0;
        try {
            for (int i=0; i < month; i++) predays += DayPerMonth(i);
        }
        catch (RuntimeException e) {return -1;};
        System.out.println(day + " " + month + " " + year + " " + leapyears + " " + predays);
        int total = (year - 1900) * 365 + day + predays + leapyears;
        int dayOfWeek = (total % 7 + 7) % 7;
        return dayOfWeek;
    }
}