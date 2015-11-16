--   Question: 2
--   The strategy behind the formulation of my answer: Beginning with a class,
-- I joined with CS professors, then found the prerequisites and joined those
-- classes as well as their professors with the non-CS and term constraints.

SELECT class.cno AS CNO,
       professor.pname AS CSNAME,
       preclass.cno AS prereqCNO,
       preprof.pname AS NonCSNAME
FROM class
INNER JOIN professor ON professor.eid = class.instructor
    AND professor.dept = 'Computer Science'
INNER JOIN prerequisite ON prerequisite.cno = class.cno
INNER JOIN class preclass ON preclass.cno = prerequisite.prereq
    AND preclass.term = 'S03'
INNER JOIN professor preprof ON preprof.eid = preclass.instructor
    AND preprof.dept != 'Computer Science'
WHERE class.term = 'S03'
GROUP BY class.cno, professor.pname, preclass.cno, preprof.pname
ORDER BY class.cno, professor.pname, preclass.cno, preprof.pname;

-- CNO         CSNAME      prereqCNO   NonCSNAME
-- ----------  ----------  ----------  ----------
-- CS214       JERALD      PHYS171     DUSTIN
-- CS348       RODRIGO     MATH217     JESSICA
-- CS436       RODRIGO     MATH381     PETER
-- CS446       LINA        PHYS171     DUSTIN
