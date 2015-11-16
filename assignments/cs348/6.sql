--   Question: 6
--   The strategy behind the formulation of my answer: We construct a common
-- table expression containing all courses with their associated professor,
-- class size, and class average. Then, we simply aggregate this table with
-- itself such that the minimum of the agreggate's size is 10 (ie. such that
-- the smallest class size is 10).

WITH classes AS (
    SELECT professor.pname,
           class.cno,
           class.term,
           class.section,
           COUNT(enrollment.sno) AS size,
           ROUND(AVG(enrollment.mark), 2) AS average
    FROM professor
    INNER JOIN class ON class.instructor = professor.eid
    INNER JOIN enrollment ON enrollment.cno = class.cno
        AND enrollment.term = class.term
        AND enrollment.section = class.section
    GROUP BY professor.eid, class.cno, class.term, class.section
)

SELECT classes.pname,
       classes.cno,
       classes.term,
       classes.section,
       classes.size,
       classes.average
FROM classes
INNER JOIN classes d2 ON d2.pname = classes.pname
GROUP BY classes.pname, classes.cno, classes.term, classes.section
HAVING MIN(d2.size) >= 10

-- pname       cno         term        section     size        average
-- ----------  ----------  ----------  ----------  ----------  ----------
-- DENIS       CS136       F01         2           32          69.63
-- DENIS       CS230       F01         2           37          70.97
-- DENIS       CS234       F99         3           36          69.64
-- DENIS       CS247       F01         1           42          70.57
-- DENIS       CS436       F99         3           56          72.7
-- DENIS       CS442       F01         3           33          71.27
-- DENIS       CS444       F01         1           45          72.16
-- DENIS       CS447       F99         3           66          69.52
-- GAYLA       PHYS133     F99         2           76          73.62
-- GAYLA       PHYS243     F01         3           65          72.18
-- MARLA       MATH225     F01         1           39          69.26
-- MARLA       MATH245     F01         2           47          74.06
-- MARLA       MATH340     F01         3           44          72.73
-- MARLA       MATH340     F99         2           71          72.0
-- MARLA       MATH362     F99         2           58          69.24
-- MARLA       MATH362     S02         2           33          73.88
-- MARLA       MATH395     F99         2           77          71.43
-- MARLA       MATH428     F01         2           44          70.68
-- MARLA       MATH439     F01         2           58          70.05
-- MARLA       MATH476     F01         2           68          71.94
-- MARLA       MATH476     F99         3           95          71.73
-- MARLA       MATH479     F99         2           80          70.56
-- MOLLIE      MATH197     F01         2           46          71.87
-- MOLLIE      MATH221     F99         2           49          70.45
-- MOLLIE      MATH245     F99         1           72          70.72
-- MOLLIE      MATH362     F05         2           32          73.63
-- MOLLIE      MATH362     F99         3           64          72.02
-- MOLLIE      MATH362     S06         2           33          74.48
-- MOLLIE      MATH381     F01         2           55          71.25
-- MOLLIE      MATH381     F01         3           63          71.0
-- MOLLIE      MATH427     F01         2           38          72.24
-- MOLLIE      MATH427     F99         2           73          69.6
-- MOLLIE      MATH428     F01         3           64          70.19
-- MOLLIE      MATH492     F99         2           70          72.3
-- ZELDA       MATH225     F01         2           45          68.8
-- ZELDA       MATH250     F99         2           74          71.78
-- ZELDA       MATH334     F01         2           46          72.57
-- ZELDA       MATH399     F01         1           53          72.08
-- ZELDA       MATH424     F99         3           65          72.05
-- ZELDA       MATH439     F99         2           85          72.26
-- ZELDA       MATH474     F01         3           54          71.72
-- ZELDA       MATH479     F01         1           70          70.89
-- ZELDA       MATH492     F01         1           51          70.76
