import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

import java.util.Properties;
import java.util.Scanner;

public class CourseInfo {
    private static Connection createConnection() {
        try {
            Class.forName("com.ibm.db2.jcc.DB2Driver");
        } catch (ClassNotFoundException e) {
            System.err.println("No DB2 driver found.");
            System.exit(-1);
        }

        Properties props = new Properties() {
            private static final long serialVersionUID = 123456789L;

            {
                put("currentSchema", "ENROLLMENT");
                put("user", "USERNAME_HIDDEN");
                put("password", "PASSWORD_HIDDEN");
            }
        };

        Connection conn = null;
        try {
            conn = DriverManager.getConnection(
                "jdbc:db2://linux.student.cs.uwaterloo.ca:50002/cs348", props);
        } catch (Exception e) {
            System.err.println("Connection failure: " + e);
            System.exit(-1);
        }

        return conn;
    }

    private static PreparedStatement createPreparedStatement(Connection conn) {
        PreparedStatement stmt = null;
        try {
            stmt = conn.prepareStatement(
                "WITH averages AS ( " +
                "    SELECT enrollment.cno, " +
                "           enrollment.term, " +
                "           enrollment.section, " +
                "           AVG(enrollment.mark) AS average_mark, " +
                "           SUM(enrollment.mark) AS total_mark, " +
                "           COUNT(*) AS enrollments " +
                "    FROM enrollment " +
                "    GROUP BY enrollment.cno, enrollment.term, enrollment.section " +
                ") " +
                "SELECT class.cno, " +
                "       course.cname, " +
                "       SUBSTR(class.term, 2) AS year, " +
                "       COUNT(class.term) AS sections, " +
                "       SUM(averages.enrollments) AS enrollments, " +
                "       MIN(averages.average_mark) AS min_average, " +
                "       MAX(averages.average_mark) AS max_average, " +
                "       SUM(averages.total_mark) / SUM(averages.enrollments) AS overall_average " +
                "FROM class " +
                "INNER JOIN course ON course.cno = class.cno " +
                "INNER JOIN averages ON averages.cno = class.cno " +
                "    AND averages.term = class.term " +
                "    AND averages.section = class.section " +
                "INNER JOIN professor ON professor.eid = class.instructor " +
                "    AND professor.dept = ? " +
                "WHERE ? <= SUBSTR(class.term, 2) " +
                "    AND SUBSTR(class.term, 2) <= ? " +
                "GROUP BY class.cno, course.cname, SUBSTR(class.term, 2) " +
                "ORDER BY SUBSTR(class.term, 2), class.cno "
            );
        } catch (SQLException e) {
            System.err.println("SQL Error: " + e);
            System.exit(-1);
        }

        return stmt;
    }

    public static void main(String[] args) {
        Connection conn = createConnection();
        PreparedStatement stmt = createPreparedStatement(conn);

        Scanner reader = new Scanner(System.in);
        ResultSet rs = null;
        String dept;
        int begin, end;
        while(true) {
            System.out.print("Enter a department name (or exit): ");
            dept = reader.next();
            if (dept.equals("exit")) {
                break;
            }

            System.out.print("Enter a start year: ");
            begin = reader.nextInt();

            System.out.print("Enter a last year: ");
            end = reader.nextInt();

            try {
                stmt.setString(1, dept);
                stmt.setInt(2, begin - 1900);
                stmt.setInt(3, end - 1900);

                rs = stmt.executeQuery();

                System.out.format("%-5s %-39s%11s%9s%11s%14s%14s\n", "C#", "Name",
                                  "Enrollment", "#Section", "Course Ave",
                                  "Max Class Ave", "Min Class Ave");
                while (rs.next()) {
                    System.out.format("%-5s %-39s%11s%9s%11s%14s%14s\n",
                                      rs.getString("cno"),
                                      rs.getString("cname"),
                                      rs.getString("enrollments"),
                                      rs.getString("sections"),
                                      rs.getString("overall_average"),
                                      rs.getString("max_average"),
                                      rs.getString("min_average")
                                      );
                }
            } catch (SQLException e) {
                System.err.println("SQL Error: " + e);
                System.exit(-1);
            }
        }

        try {
            conn.close();
        } catch (SQLException e) {
            return;
        }
    }
}
