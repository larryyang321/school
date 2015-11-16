--   Question: 3
--   The strategy behind the formulation of my answer: Beginning with the class
-- table, I join in the professor to return that data. Then, using the
-- prerequisite table, I join in the data for the preqequisite classes. Finally,
-- I group by advanced classes having a large enough enrollement and a great
-- enough mark.

SELECT professor.pname,
       professor.dept,
       class.cno,
       class.term,
       class.section,
       ROUND(AVG(enrollment.mark), 2) AS average
FROM class
INNER JOIN professor ON professor.eid = class.instructor
LEFT JOIN enrollment ON enrollment.cno = class.cno
    AND enrollment.term = class.term
    AND enrollment.section = class.section
GROUP BY class.cno, class.term, class.section
HAVING COUNT(enrollment.sno) >= 10
    AND average > 85
ORDER BY professor.pname, professor.dept, class.cno;

-- PNAME       DEPT              CNO         TERM        SECTION     average
-- ----------  ----------------  ----------  ----------  ----------  ----------
-- ANN         Computer Science  CS213       F11         2           85.92
-- BARBARA     Computer Science  CS137       S02         3           86.85
-- BARBARA     Computer Science  CS137       W09         3           85.6
-- BARBARA     Computer Science  CS213       S04         1           86.0
-- BARBARA     Computer Science  CS213       W00         2           85.7
-- BARBARA     Computer Science  CS483       S13         2           85.08
-- BETH        Physics           PHYS457     F01         2           86.64
-- CODY        Computer Science  CS138       F00         3           85.6
-- DEBORA      Physics           PHYS451     F10         1           86.83
-- DUSTIN      Physics           PHYS214     W12         2           86.18
-- DUSTIN      Physics           PHYS271     W13         3           86.78
-- DUSTIN      Physics           PHYS477     W11         3           86.0
-- HOLLIS      Computer Science  CS484       F06         2           86.1
-- JACKLYN     Physics           PHYS147     W07         2           86.09
-- JERALD      Computer Science  CS138       F10         2           85.08
-- JERALD      Computer Science  CS489       W13         1           85.5
-- JESSICA     Mathematics       MATH226     F12         1           85.55
-- JESSICA     Mathematics       MATH285     W03         3           87.8
-- JESSICA     Mathematics       MATH358     W02         1           85.08
-- JESSICA     Mathematics       MATH399     F09         1           85.1
-- JESSICA     Mathematics       MATH439     F12         1           85.4
-- LILIA       Physics           PHYS189     F11         2           86.92
-- LINA        Computer Science  CS241       W04         2           85.8
-- MORRIS      Physics           PHYS428     S13         3           85.17
-- MORRIS      Physics           PHYS442     S03         1           85.3
-- PETER       Mathematics       MATH358     W00         1           86.25
-- PETER       Mathematics       MATH452     F08         3           86.73
-- RODRIGO     Computer Science  CS135       S11         2           86.0
-- RODRIGO     Computer Science  CS446       W03         2           87.7
-- SHAUNA      Physics           PHYS113     S10         3           85.6
-- STUART      Physics           PHYS289     F09         2           85.5
-- STUART      Physics           PHYS414     W04         2           86.2
-- ULYSSES     Physics           PHYS312     F11         1           85.08
-- ULYSSES     Physics           PHYS331     S05         1           86.73
-- ULYSSES     Physics           PHYS363     S13         2           88.09
