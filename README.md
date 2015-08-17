SQL_ExamSystem
==============

An ExamSystem based on SQL Server &amp; win32, written mostly by Win32 API.

**End on 16 May 2014.**

# 系统分析

## 课题背景

随着标准化考试的日益普及，在各种考试中越来越多地采用了计算机考试的方式。相对于传统的卷面考试而言，由于传统的卷面考试从出题印刷到下发试卷等环节需要较长的时间，接触的人员相对较多，因而在保密方面具有一定的困难。而计算机考试系统采用试题库方式提供试题来源，考前无任何成套试卷，考试时考卷由计算机现场随机生成，各考生试卷不完全相同，从而避免了互相参看等作弊现象或不公平成绩的出现。同时也可考后即可获得成绩，省去了人力评卷的麻烦和弊端。

## 目的和意义

随着网络化考试和无纸化考试进程的推进，通过在线考试系统考试是大势所趋。

本系统的目的在于为考试提供完善的系统，不仅要能做到简单的组卷、安排考试、卷等功能，还要注重信息的安全性，因为本系统是在线上的考试系统，所以要能抵御用户提交的各种非法操作，对于合法操作也要能快速地响应。对于权限问题，也是应该严格控制的，对管理员的每一项权限都进行了管理，没有像一般系统那样的用户组设定，而是按照统一冠以管理员权限但对每一项权限都经过设定并且严加控制的方式管理。

本系统的意义在于搭建一个定制化低、扩展性高、安全性强、功能强大的在线考试系统，弥补现有系统的不足。

## 可行性分析

对于在线考试系统需要完成的功能，对其简单介绍如下：

1. 管理员管理。系统在初始化时会保留一个超级用户su，用于初始化最初的管理员以及分配管理员的各项权限。管理员的管理由拥有相应权限的管理员进行管理。
2. 考生管理。系统对于考试开放了注册接口，允许考生向数据库中注册自己的信息，并且登陆系统参加考试以及管理自己的信息。考生的管理由拥有相应权限的管理员进行管理。
3. 分类管理。考试卷的分类需要管理，因为试题的分配将按照分类进行。这部分由拥有相关权限的管理员进行管理。
4. 题目管理。由于是在线自动组卷考试，需要在安排考试前先录入题目，题目主要有难度属性，用于组卷。题目的设定由拥有相应权限的管理员进行管理。
5. 试卷管理。由于是自动组卷，试卷的管理将需要设定各个难度级别的题目各有多少数量，以及相应分值。试卷的设定由拥有相应权限的管理员进行管理。
6. 考试管理。每场考试对应一份卷子，由于在线考试不全要求考试的同时性，所以本系统可以通过设定考试的开始时间、结束时间、持续时间构成各种要求的考试。考试的设定由拥有相应权限的管理员进行管理。
7. 答案管理。这部分将记录考生答题的答案，而且是由系统自动管理，不允许管理员干涉的。
8. 成绩管理。在考生交卷后自动计算。但是考虑到各种原因，成绩允许删除或修改，但是只能由拥有相应权限的管理员进行管理。

在线考试系统的业务流程图如图所示：

![1.jpg](https://raw.githubusercontent.com/MewX/sql-exam-system/master/screenshots/1.jpg)

# 系统设计

## 数据流图

数据流图是以图形方式描绘数据在系统中流动和处理的过程，由于它只反映系统必须完成的逻辑功能，所以它是一种功能模型。

系统基本信息模块是管理员对在线考试系统的基础信息进行管理，各种信息只要有权限均可进行增删改操作。该数据流图如图：

![2.jpg](https://raw.githubusercontent.com/MewX/sql-exam-system/master/screenshots/2.jpg)

考生模块是考生对自己信息的修改以及对考试的动作。其中包括可选的考试考生自愿参加以及强制的考试考生参加。该数据流图如图所示：

![3.jpg](https://raw.githubusercontent.com/MewX/sql-exam-system/master/screenshots/3.jpg)

考试模块是考生参加考试所做出的动作以及数据流动。其中包括考生抽取题目、保存答案、提交答案，考试系统的选题、发题、计时、保存答案、计算分数等。该数据流图如图所示：

![4.jpg](https://raw.githubusercontent.com/MewX/sql-exam-system/master/screenshots/4.jpg)

成绩管理模块是考试后对考生成绩的处理过程。其中包括考生自己提交答案后的自动提交分数以及管理员删除、修改考生的考试分数。该数据流图如图所示：

![5.jpg](https://raw.githubusercontent.com/MewX/sql-exam-system/master/screenshots/5.jpg)

## 数据字典

略

## 系统模块总体设计

本系统主要分为三个模块，每个模块之间都紧密联系，但是又维护着数据的独立性。数据库整体设计符合BC范式设计要求。

依据上述的功能分析，系统中的模块分为：系统基本信息管理模块、考生模块、考试模块、成绩管理模块。

各模块的具体功能如下：

1. 系统基本信息管理模块。该模块权限交由管理员，管理员除了可以维护自己的个人资料外，还可以依据自己被高权管理员赋予的权限有限制性地进行管理员管理、考生管理、试题管理、试卷管理、考试管理、成绩管理。
2. 考生模块。该模块由考生自行维护，考生登陆管理后台后可以修改自己的账号信息，以及查看可参加的考试以及必须参加的考试信息。对于可参加的考试需要通过管理后台预先报名；对于必须参加的考试，考生需要查明考试时间及相关信息。
3. 考试模块。该模块是处理考生在考试中所有事务的模块。由于是在线考试，考试的自由度会比正常考试要高，本系统允许在考试中暂停考试，下一次登陆系统时继续考试。所以本模块展现给考生的题目是一道、一道的，做完一道保存一道，对于可暂停的考试要保证考生不能看到所有题目且已保存的答案不能再次修改。而且对于时间的限制要精确，时间一到将自动保存并提交答案、计算成绩。
4. 成绩管理模块。成绩管理一部分是由系统自动维护，另一部分是交给有相应权限的管理员维护。自动维护的方式是考试结束后统一批卷并向学生下发成绩信息；管理员维护是在特殊情况下允许干预成绩的计算，但是要记录在日志中，因为这部分的权限数据高敏感权限。

系统功能结构图如图所示：

![6.jpg](https://raw.githubusercontent.com/MewX/sql-exam-system/master/screenshots/6.jpg)

## 数据库概念结构设计

E-R方法是描述现实世界概念结构模型的有效方法。是表示概念模型的一种方式，用矩形表示实体型，矩形框内写明实体名；用椭圆表示实体的属性，并用无向边将其与相应的实体型连接起来；用菱形表示实体型之间的联系，在菱形框内写明联系名，并用无向边分别与有关实体型连接起来，同时在无向边旁标上联系的类型。

数据库系统设计的第一步就是要明确各数据表、数据项之间的关系，为了表示这种关系我们采用E-R图的方式。表示完成后就可以开始建立数据库了。

系统E-R图如图所示：

![7.jpg](https://raw.githubusercontent.com/MewX/sql-exam-system/master/screenshots/7.jpg)

# 系统实现

## 数据库逻辑结构设计

下面是表的创建代码及注释：

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
      canSetPaper bit default 1,
      canSetExam bit default 1,
      canDeleteGrade bit default 1,
      canEditGrade bit default 0        -- Edit answers and grades calculated.
    );

    create table Student (
      StuId int primary key,             -- max( ? ) + 1
      StuName varchar( 20 ) not null,
      StuPassword char( 64 ) not null,
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
      PaperSpec int,    -- Paper Classification. In fact, we use this with "Question" table

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
      constraint ValueLegalityCHK check ( ACount >= 0 and BCount >= 0 and CCount >= 0
             and DCount >= 0 and AScore > 0 and BScore > 0 and CScore > 0 and DScore > 0 )
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
      ActualDuring int default 0, -- Student can pause an exam, so this time should also be stored
      LastStoredTime datetime default null,
      SubmitTime datetime default null,

      constraint GradePK primary key ( StuId, ExamId ),
      constraint GradeStuFK foreign key ( StuId ) references Student( StuId ),
      constraint GradeExamFK foreign key ( ExamId ) references Exam( ExamId )
    );

    -- ----------------------------
    -- Create Views
    -- ----------------------------
    create view Admin_General
    as select AdminName, AdminType, canManageAdmin, canManageStudent, canSetProblem, canSetPaper,
    canSetExam, canDeleteGrade, canEditGrade from Admin;
    create view Student_General as select StuName, ExamTime from Student;


## 系统界面设计

- 本系统出于安全性考虑，所有操作都要记录日志文件，所以必须有一个身份标识，那就是登陆状态。系统的登陆界面如图所示，管理员和考生均是这个登陆界面。在这个登陆界面中，客户端首先会尝试连接到服务器如果连接失败，所有的窗口控件除了“Retry”按钮，都将被禁用，只有当连接服务器成功才能进行登录和注册操作。

  ![8.jpg](https://raw.githubusercontent.com/MewX/sql-exam-system/master/screenshots/8.jpg)

- 系统的对于注册设定是这样的：不允许注册管理员，只能注册考生用户组。管理员账号将只能由系统保留的超级用户或者有权限的管理员账号内建。注册界面如图：

  ![9.jpg](https://raw.githubusercontent.com/MewX/sql-exam-system/master/screenshots/9.jpg)

- 登录系统后不同的用户看到的后台界面将不同。管理员看到的将是管理员后台。学生看到的将是考生后台。

  管理员后台界面如图所示。在登陆时会获取已登录管理员的权限，然后会在后台显示拥有管理权限的相关标签，选中某一个标签页即进入了某个表的管理状态，可以进行权限范围内的修改、删除、创建操作。

  ![10.jpg](https://raw.githubusercontent.com/MewX/sql-exam-system/master/screenshots/10.jpg)

- 考生后台如图所示。只是负责功能的引导，未完成的考试包括必须参加的、自己报名的、以及暂停的考试。

  ![11.jpg](https://raw.githubusercontent.com/MewX/sql-exam-system/master/screenshots/11.jpg)

- 系统的核心在于考试系统，考试界面如图所示。这部分业务逻辑比较复杂，因为考试存在暂停功能。对于暂停过的考试，已阅的题答案不允许修改，但是允许返回查看当时选择的答案。每次下一题都是随机取题，增加考试的随机性和公平性。考试中系统也会自动保存答案，防止答题数据丢失，更防止非法抓题目包，所有发往客户端的题都将写入答题数据库，不会存在用户非法抓题的可能；这样也能防止用户遇到不会做的题目而非法中断考试进程。

  ![12.jpg](https://raw.githubusercontent.com/MewX/sql-exam-system/master/screenshots/12.jpg)


# 总结

## 设计体会

通过本次课程设计的契机，我实战使用Win32 API + ODBC API制作了一个DEMO程序，由于是使用C++直接调用Win32 API，代码量非常大，本系统并没有完工。但是在制作的过程中我基本掌握了ODBC API的使用方法，可以用这种较底层的方式操作数据库让我感到非常高兴，因为越是底层越能直接接触本质、越能理解实现原理。

在实战中，我尝试用非常规手段攻击自己的程序，并且成功破坏了数据库中的数据，这样让我更加深刻地体会到了数据库的安全性能的重要性。在提供给用户的接口中要非常谨慎，而且在服务端的程序要做好数据的校验，不能让恶意工具有任何可乘之机。当然，自己代码的缺陷也要尽可能避免，要尽一切可能保障数据的安全性。

在交流中，对于数据的检查应尽可能地放在软件层面上，而不是通过大量在数据库系统中定义触发器这样的方式。所以我在实践中极力保证这个设计思路，在客户端中做了绝大部分的检查工作。在我亲自体验其他公司产品时，我发现这些公司的产品大多不仅有客户端检查，还有服务端检查，因为跳过本机验证上传了非法数据还是会提交不通过。所以我也在相应

在数据库的设计中，我使用了MySQL做兼容性测试，发现一切良好，只需要改动一些类型名称和系统函数名称就可以移植了。但是不能一字不动地直接移植真是很大的遗憾。

另外，关于绘图。绘图软件我以前用过Enterprise Architect，它可以通过代码生成UML图；我也用过Erwin通过sql脚本生成E-R图。这次第一次使用MS Visio，而且绘制了所有的图，感觉这款软件自由度还是比较大的，虽然自动化功能不是很强大，但是在手动的情况下确实是非常值得考虑的选择。

## 系统改进

作为一个模拟C/S架构的系统程序，从原理上说应该制作两个程序，一个是服务端（可在本机开端口模拟），另一个是客户端程序。而本次试验的DEMO只是在类的划分上区分开客户部分和服务部分，不够完美也不够仿真。

由于采用ODBC的缘故，在数据源没有设定的情况下就完全不能工作了。这点上最好应该能实现全自动初始化的工作。

作为跨平台和开源的爱好者，这个系统最好能移植到Linux和UNIX上运行，所以可以考虑用Qt或者其他界面接口开发。
