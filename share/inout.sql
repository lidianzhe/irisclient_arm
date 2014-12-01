PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS InoutDetails
(
	PId integer not null primary key autoincrement,
	DeviceNo varchar(20) ,
	PersonId int null,
	CardTime datetime,
	Flag tinyint,
	SeriesId tinyint,
	if_UserNo integer
);
COMMIT;

