include ../../latex.Makefile


all: a2.pdf a3.pdf a4.pdf a5.pdf CourseInfo
a2.pdf: a2.tex a2schema.png
a4.pdf: 1.sql 2.sql 3.sql 4.sql 5.sql 6.sql
a5.pdf: CourseInfo.java
CourseInfo: CourseInfo.java db2jcc4.jar db2jcc_license_cu.jar
	javac $<

clean:
	rm -f *.class
	rm -f a2.pdf
	rm -f a3.pdf
	rm -f a4.pdf
	rm -f a5.pdf
