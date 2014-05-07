/**
 * Name: SQL Query for SQL_ExamSystem
 * Author: MewCatcher
 * Date: 2014/05/07
 * Description: This .sql file inits the environment for the C++Program.
 * Note: Super user info (su/1), save as(su/be9d9be4e8df0f4046100680ff3b209c15715e6c9f1057ca945c08cfc77b2a50)
 **/


-- ----------------------------
-- Create Database
-- ----------------------------
create database MewCatcher_ExamSystem;
use MewCatcher_ExamSystem;


-- ----------------------------
-- Create Tables
-- ----------------------------
create table Admin (
  AdminId int primary key,           -- max( ? ) + 1
  AdminName varchar( 20 ) not null,  -- Letters and Numbers Only, not case sensitive
  AdminPassword char( 64 ) not null, -- Hash 256 digested ( Letters and Numbers Only )
  RegTime datetime default getdate( ),   -- Set automatically, SQL Server/getdate( )
  AdminType bit default 1,                -- 0/Super User; 1/Admin. (Only one SuperUser: su, 1)
  
  canManageAdmin bit default 0,      -- 0/cannot; 1/can
  canManageStudent bit default 1,
  canSetProblem bit default 1,
  canSetPaper bit default 0,
  canDeleteGrade bit default 1,
  canEditGrade bit default 0        -- Edit answers and grades calculated.
);

create table Student (
  StuId int primary key,             -- max( ? ) + 1
  StuName varchar( 20 ) not null,
  StuPassword char( 32 ) not null,
  RegTime datetime default getdate( ),

  ExamTime int default 0             -- count( ? ) where ?=StuId
);

create table Specification (                  -- Specification of question
  SpecId int primary key,            -- Added by program automatically
  SpecName varchar( 20 ) not null    -- Get from user
);

create table Question (
  QuestId int primary key,
  QuestDetail varchar( 500 ) not null,
  AnswerA varchar( 100 ) not null,
  AnswerB varchar( 100 ) not null,
  AnswerC varchar( 100 ) not null,
  AnswerD varchar( 100 ) not null,

  Difficulty int default 1,           -- 1 ~ 4 ( easy ~ hard )
  Score int default 4,                -- upper than 1
  Spec int,                           -- Classify the quest into several types
  
  CreateTime datetime default getdate( ),
  CreateBy int,

  constraint QuestionCreateByFK foreign key ( CreateBy ) references Admin( AdminId ),
  constraint SpecFK foreign key ( Spec ) references Specification( SpecId )
);

create table Paper (
  PaperId int primary key,
  PaperName varchar( 100 ) not null,  -- Paper Name
  PaperSpec int,                      -- Paper Classification. In fact, we use this with "Question" table

  ACount int not null,                -- 'A' difficulty quest count
  BCount int not null,
  CCount int not null,
  DCount int not null,

  AScore int not null,                -- Each specific difficulty quest values a score
  BScore int not null,
  CScore int not null,
  DScore int not null,
  -- TotalScore int default ACount*AScore + BCount*BScore + CCount*CScore + DCount*DScore,

  CreateTime datetime default getdate( ),
  CreateBy int,

  constraint PaperCreateByFK foreign key ( CreateBy ) references Admin( AdminId ),
  constraint PaperSpecFK foreign key ( PaperSpec ) references Specification( SpecId ),
  constraint ValueLegalityCHK check ( ACount >= 0 and BCount >= 0 and CCount >= 0 and DCount >= 0
                                  and AScore > 0 and BScore > 0 and CScore > 0 and DScore > 0 )
);

create table Exam (
  ExamId int primary key,
  ExamName varchar( 100 ) not null,
  PaperRef int,

  ExamTimeBegin datetime not null,    -- 0 refers anytime available
  ExamTimeEnd datetime not null,
  ExamDuring int not null,            -- ? minutes, user can pause it while quizing

  CreateTime datetime default getdate( ),
  CreateBy int,

  constraint ExamCreateByFK foreign key ( CreateBy ) references Admin( AdminId ),
  constraint PaperRefFK foreign key ( PaperRef) references Paper( PaperId ),
  constraint ValueCHK check ( DateDiff( minute, ExamTimeBegin, ExamTimeEnd ) > ExamDuring )
);

create table Answer (                -- Once starting an exam, all the problem set null
  StuId int,
  ExamId int,
  QuestId int,
  StuAnswer int,                     -- A ~ D ( 1~4 )

  constraint AnswerPK primary key ( StuId, ExamId, QuestId ),
  constraint AnsStuFK foreign key ( StuId ) references Student( StuId ),
  constraint AnsExamFK foreign key ( ExamId ) references Exam( ExamId ),
  constraint AnsQuestFK foreign key ( QuestId ) references Question( QuestId )
);

create table Grade (
  StuId int,
  ExamId int,
  GradeCalc int,

  StartTime datetime default getdate( ),
  ActualDuring int default 0,        -- Student can pause an exam, so this time should also be stored
  LastStoredTime datetime default null,
  SubmitTime datetime default null,

  constraint GradePK primary key ( StuId, ExamId ),
  constraint GradeStuFK foreign key ( StuId ) references Student( StuId ),
  constraint GradeExamFK foreign key ( ExamId ) references Exam( ExamId )
);


-- ----------------------------
-- Init Tables with values
-- ----------------------------
insert into Admin( AdminId, AdminName, AdminPassword, AdminType,
                   canManageAdmin, canManageStudent, canSetProblem, canSetPaper, canDeleteGrade, canEditGrade )
values ( 1, 'su', 'be9d9be4e8df0f4046100680ff3b209c15715e6c9f1057ca945c08cfc77b2a50', 0, 1, 1, 1, 1, 1, 1 );

insert into Specification( SpecId, SpecName ) values ( 1, 'Computer Science' );

insert into Question( QuestId, QuestDetail, AnswerA, AnswerB, AnswerC, AnswerD, Difficulty, Score, Spec, CreateBy )
values ( 1, 'Test Quest 1', 'Wrong', 'Correct', 'Wrong', 'Wrong', 1, 5, 1, 1 );
insert into Question( QuestId, QuestDetail, AnswerA, AnswerB, AnswerC, AnswerD, Difficulty, Score, Spec, CreateBy )
values ( 2, 'Test Quest 2', 'Wrong', 'Wrong', 'Correct', 'Wrong', 1, 5, 1, 1 );
insert into Question( QuestId, QuestDetail, AnswerA, AnswerB, AnswerC, AnswerD, Difficulty, Score, Spec, CreateBy )
values ( 3, 'Test Quest 3', 'Wrong', 'Wrong', 'Correct', 'Wrong', 1, 5, 1, 1 );
insert into Question( QuestId, QuestDetail, AnswerA, AnswerB, AnswerC, AnswerD, Difficulty, Score, Spec, CreateBy )
values ( 4, 'Test Quest 4', 'Correct', 'Wrong', 'Wrong', 'Wrong', 1, 5, 1, 1 );

insert into Paper( PaperId, PaperName, PaperSpec, ACount, BCount, CCount, DCount, AScore, BScore, CScore, DScore, CreateBy )
values ( 1, 'Test Paper 1', 1, 4, 0, 0, 0, 25, 1, 1, 1, 1 );

insert into Exam( ExamId, ExamName, PaperRef, ExamTimeBegin, ExamTimeEnd, ExamDuring, CreateBy )
values ( 1, 'Test Exam 1', 1, 0, 200, 20, 1 ); -- 1900-01-01 00:00:00.000 ~ 1900-07-20 00:00:00.000
insert into Exam( ExamId, ExamName, PaperRef, ExamTimeBegin, ExamTimeEnd, ExamDuring, CreateBy )
values ( 2, 'Test Exam 2', 1, 0, 200, 20, 1 ); -- 1900-01-01 00:00:00.000 ~ 1900-07-20 00:00:00.000

select * from Question;
select * from Admin;
