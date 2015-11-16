--   Question: 1
--   The strategy behind the formulation of my answer: I filtered down the
-- class list by joining in courses, professors, and schedules.

SELECT course.cname,
       class.cno,
       professor.pname
FROM class
INNER JOIN course ON course.cno = class.cno
INNER JOIN professor ON professor.eid = class.instructor
INNER JOIN schedule ON schedule.cno = class.cno
    AND schedule.term = class.term
    AND schedule.section = class.section
    AND schedule.room = 'RCH122'
WHERE class.term = 'F10'
ORDER BY class.cno;

-- CNAME                                                CNO         PNAME
-- ---------------------------------------------------  ----------  ----------
-- Elementary Algorithm Design and Data Abstraction...  CS146       HOLLIS
-- Computer Architecture                                CS450       BARBARA
-- Computer Security and Privacy                        CS458       HOLLIS
-- Advanced Biochemistry                                PHYS442     LILIA
-- Research Project                                     PHYS451     DEBORA
