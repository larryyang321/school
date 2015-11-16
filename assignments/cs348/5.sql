--   Question: 5
--   The strategy behind the formulation of my answer: Beginning with students,
-- I look at their enrollments and take only those with at least five enrollments
-- for which they had a minimum mark of 80 across all enrollments.

SELECT student.sname,
       ROUND(AVG(enrollment.mark), 2) AS average
FROM student
INNER JOIN enrollment ON enrollment.sno = student.sno
GROUP BY student.sno
HAVING COUNT(enrollment.cno) >= 5
    AND MIN(enrollment.mark) >= 80
ORDER BY student.sname, average;

-- SNAME         average
-- ------------  ----------
-- ANITA, STEVE  84.0
-- CHRISTIAN, R  86.0
-- KAREN, BENJA  87.4
-- SETH, GERMAI  82.33
-- SUSIE, VIRGI  84.0
