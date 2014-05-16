use MewCatcher_ExamSystem;

------------------------------
-- Clear All
------------------------------
drop view Student_General;
drop view Admin_General;

drop table Grade;
drop table Answer;
drop table Exam;
drop table Paper;
drop table Question;
drop table Specification;
drop table Student;
drop table Admin;

use master;
drop database MewCatcher_ExamSystem;
