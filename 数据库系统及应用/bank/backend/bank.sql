/*==============================================================*/
/* DBMS name:      MySQL 5.0                                    */
/* Created on:     2022/6/9 15:22:46                            */
/*==============================================================*/

use bank;


drop table if exists bankAccount;

drop table if exists checkAccount;

drop table if exists checkClient;

drop table if exists client;

drop table if exists clientLoan;

drop table if exists department;

drop table if exists depositAccount;

drop table if exists depositClient;

drop table if exists loan;

drop table if exists loanPay;

drop table if exists staff;

drop table if exists staffClientContact;

drop table if exists subbranch;

drop table if exists visit;

/*==============================================================*/
/* Table: bankAccount                                               */
/*==============================================================*/
create table bankAccount
(
   accountID            char(30) not null,
   accountRemain        float(8,2) not null,
   accountOpenDate      date not null,
   primary key (accountID)
);

/*==============================================================*/
/* Table: checkAccount                                          */
/*==============================================================*/
create table checkAccount
(
   accountID            char(30) not null,
   subbranchName        char(20) not null,
   accountRemain        float(8,2) not null,
   accountOpenDate      date not null,
   overdraft            float(8,2) not null,
   primary key (accountID)
);

/*==============================================================*/
/* Table: checkClient                                           */
/*==============================================================*/
create table checkClient
(
   accountID            char(30) not null,
   clientID             char(20) not null,
   primary key (clientID, accountID)
);

/*==============================================================*/
/* Table: client                                                */
/*==============================================================*/
create table client
(
   clientID             char(20) not null,
   clientName           char(10) not null,
   clientPhone          numeric(8,0) not null,
   clientAddress        char(50),
   clientLinkmanName    char(10) not null,
   clientLinkmanPhone   numeric(8,0) not null,
   clientLinkmanEmail   char(40),
   clientLinkmanRelation char(10),
   primary key (clientID)
);

/*==============================================================*/
/* Table: clientLoan                                            */
/*==============================================================*/
create table clientLoan
(
   loanID               numeric(8,0) not null,
   clientID             char(20) not null,
   primary key (loanID, clientID)
);

/*==============================================================*/
/* Table: department                                            */
/*==============================================================*/
create table department
(
   departmentID         numeric(8,0) not null,
   departmentName       char(10) not null,
   departmentType       char(10) not null,
   departmentManagerID  char(20) not null,
   primary key (departmentID)
);

/*==============================================================*/
/* Table: depositAccount                                        */
/*==============================================================*/
create table depositAccount
(
   accountID            char(30) not null,
   subbranchName        char(20) not null,
   accountRemain        float(8,2) not null,
   accountOpenDate      date not null,
   moneyRate            float not null,
   currencyType         char(10) not null,
   primary key (accountID)
);

/*==============================================================*/
/* Table: depositClient                                         */
/*==============================================================*/
create table depositClient
(
   accountID            char(30) not null,
   clientID             char(20) not null,
   primary key (accountID, clientID)
);

/*==============================================================*/
/* Table: loan                                                  */
/*==============================================================*/
create table loan
(
   loanID               numeric(8,0) not null,
   allMoney             float(8,2) not null,
   primary key (loanID)
);

/*==============================================================*/
/* Table: loanPay                                               */
/*==============================================================*/
create table loanPay
(
   loanID               numeric(8,0) not null,
   payDate              date not null,
   payMoney             float(8,2) not null
);

/*==============================================================*/
/* Table: staff                                                 */
/*==============================================================*/
create table staff
(
   staffID              char(20) not null,
   departmentID         numeric(8,0) not null,
   staffName            char(10) not null,
   staffPhone           numeric(8,0) not null,
   staffAddress         char(50) not null,
   staffStartDate       date not null,
   primary key (staffID)
);

/*==============================================================*/
/* Table: staffClientContact                                    */
/*==============================================================*/
create table staffClientContact
(
   clientID             char(20) not null,
   staffID              char(20) not null,
   staffClientAssociation char(30) not null,
   primary key (clientID, staffID)
);

/*==============================================================*/
/* Table: subbranch                                             */
/*==============================================================*/
create table subbranch
(
   subbranchName        char(20) not null,
   subbranchCity        char(20) not null,
   subbranchWealth      float(8,2) not null,
   primary key (subbranchName)
);

/*==============================================================*/
/* Table: visit                                                 */
/*==============================================================*/
create table visit
(
   accountID            char(30) not null,
   clientID             char(20) not null,
   visitDate            date,
   primary key (accountID, clientID)
);

alter table checkAccount add constraint FK_Inheritance_2 foreign key (accountID)
      references bankAccount (accountID) on delete restrict on update restrict;

alter table checkAccount add constraint FK_Relationship_7 foreign key (subbranchName)
      references subbranch (subbranchName) on delete restrict on update restrict;

alter table checkClient add constraint FK_checkClient foreign key (clientID)
      references client (clientID) on delete restrict on update restrict;

alter table checkClient add constraint FK_checkClient2 foreign key (accountID)
      references checkAccount (accountID) on delete restrict on update restrict;

alter table clientLoan add constraint FK_clientLoan foreign key (loanID)
      references loan (loanID) on delete restrict on update restrict;

alter table clientLoan add constraint FK_clientLoan2 foreign key (clientID)
      references client (clientID) on delete restrict on update restrict;

alter table depositAccount add constraint FK_Inheritance_1 foreign key (accountID)
      references bankAccount (accountID) on delete restrict on update restrict;

alter table depositAccount add constraint FK_Relationship_13 foreign key (subbranchName)
      references subbranch (subbranchName) on delete restrict on update restrict;

alter table depositClient add constraint FK_depositClient foreign key (accountID)
      references depositAccount (accountID) on delete restrict on update restrict;

alter table depositClient add constraint FK_depositClient2 foreign key (clientID)
      references client (clientID) on delete restrict on update restrict;

alter table loanPay add constraint FK_Relationship_6 foreign key (loanID)
      references loan (loanID) on delete restrict on update restrict;

alter table staff add constraint FK_Relationship_9 foreign key (departmentID)
      references department (departmentID) on delete restrict on update restrict;

alter table staffClientContact add constraint FK_staffClientContact foreign key (clientID)
      references client (clientID) on delete restrict on update restrict;

alter table staffClientContact add constraint FK_staffClientContact2 foreign key (staffID)
      references staff (staffID) on delete restrict on update restrict;

alter table visit add constraint FK_visit foreign key (accountID)
      references bankAccount (accountID) on delete restrict on update restrict;

alter table visit add constraint FK_visit2 foreign key (clientID)
      references client (clientID) on delete restrict on update restrict;
