USE [master]
GO
/****** Object:  Database [CollectionDB]    Script Date: 2020/3/29 13:07:23 ******/
CREATE DATABASE [CollectionDB]
 CONTAINMENT = NONE
 ON  PRIMARY 
( NAME = N'CollectionDB', FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL11.MSSQLSERVER\MSSQL\DATA\CollectionDB.mdf' , SIZE = 307200KB , MAXSIZE = UNLIMITED, FILEGROWTH = 102400KB )
 LOG ON 
( NAME = N'CollectionDB_log', FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL11.MSSQLSERVER\MSSQL\DATA\CollectionDB_log.ldf' , SIZE = 1024KB , MAXSIZE = 2048GB , FILEGROWTH = 10%)
GO
ALTER DATABASE [CollectionDB] SET COMPATIBILITY_LEVEL = 110
GO
IF (1 = FULLTEXTSERVICEPROPERTY('IsFullTextInstalled'))
begin
EXEC [CollectionDB].[dbo].[sp_fulltext_database] @action = 'enable'
end
GO
ALTER DATABASE [CollectionDB] SET ANSI_NULL_DEFAULT OFF 
GO
ALTER DATABASE [CollectionDB] SET ANSI_NULLS OFF 
GO
ALTER DATABASE [CollectionDB] SET ANSI_PADDING OFF 
GO
ALTER DATABASE [CollectionDB] SET ANSI_WARNINGS OFF 
GO
ALTER DATABASE [CollectionDB] SET ARITHABORT OFF 
GO
ALTER DATABASE [CollectionDB] SET AUTO_CLOSE OFF 
GO
ALTER DATABASE [CollectionDB] SET AUTO_CREATE_STATISTICS ON 
GO
ALTER DATABASE [CollectionDB] SET AUTO_SHRINK OFF 
GO
ALTER DATABASE [CollectionDB] SET AUTO_UPDATE_STATISTICS ON 
GO
ALTER DATABASE [CollectionDB] SET CURSOR_CLOSE_ON_COMMIT OFF 
GO
ALTER DATABASE [CollectionDB] SET CURSOR_DEFAULT  GLOBAL 
GO
ALTER DATABASE [CollectionDB] SET CONCAT_NULL_YIELDS_NULL OFF 
GO
ALTER DATABASE [CollectionDB] SET NUMERIC_ROUNDABORT OFF 
GO
ALTER DATABASE [CollectionDB] SET QUOTED_IDENTIFIER OFF 
GO
ALTER DATABASE [CollectionDB] SET RECURSIVE_TRIGGERS OFF 
GO
ALTER DATABASE [CollectionDB] SET  DISABLE_BROKER 
GO
ALTER DATABASE [CollectionDB] SET AUTO_UPDATE_STATISTICS_ASYNC OFF 
GO
ALTER DATABASE [CollectionDB] SET DATE_CORRELATION_OPTIMIZATION OFF 
GO
ALTER DATABASE [CollectionDB] SET TRUSTWORTHY OFF 
GO
ALTER DATABASE [CollectionDB] SET ALLOW_SNAPSHOT_ISOLATION OFF 
GO
ALTER DATABASE [CollectionDB] SET PARAMETERIZATION SIMPLE 
GO
ALTER DATABASE [CollectionDB] SET READ_COMMITTED_SNAPSHOT OFF 
GO
ALTER DATABASE [CollectionDB] SET HONOR_BROKER_PRIORITY OFF 
GO
ALTER DATABASE [CollectionDB] SET RECOVERY FULL 
GO
ALTER DATABASE [CollectionDB] SET  MULTI_USER 
GO
ALTER DATABASE [CollectionDB] SET PAGE_VERIFY CHECKSUM  
GO
ALTER DATABASE [CollectionDB] SET DB_CHAINING OFF 
GO
ALTER DATABASE [CollectionDB] SET FILESTREAM( NON_TRANSACTED_ACCESS = OFF ) 
GO
ALTER DATABASE [CollectionDB] SET TARGET_RECOVERY_TIME = 0 SECONDS 
GO
EXEC sys.sp_db_vardecimal_storage_format N'CollectionDB', N'ON'
GO
USE [CollectionDB]
GO
/****** Object:  Table [dbo].[Analog_Input]    Script Date: 2020/3/29 13:07:23 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[Analog_Input](
	[id] [bigint] IDENTITY(1,1) NOT NULL,
	[groupID] [nvarchar](100) NOT NULL,
	[channel] [int] NOT NULL,
	[startTime] [datetime2](3) NOT NULL,
	[frequency] [int] NOT NULL,
	[sampleCount] [int] NOT NULL,
	[sampleData] [varbinary](max) NULL,
	[isUse] [bit] NOT NULL,
	[deviceID] [int] NOT NULL,
 CONSTRAINT [PK_Analog_Input_id] PRIMARY KEY CLUSTERED 
(
	[id] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[Analog_Input_Err]    Script Date: 2020/3/29 13:07:23 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[Analog_Input_Err](
	[id] [bigint] IDENTITY(1,1) NOT NULL,
	[groupID] [nvarchar](100) NOT NULL,
	[channel] [int] NOT NULL,
	[startTime] [datetime2](3) NOT NULL,
	[frequency] [int] NOT NULL,
	[sampleCount] [int] NOT NULL,
	[sampleData] [varbinary](max) NULL,
	[isUse] [bit] NOT NULL,
	[deviceID] [int] NOT NULL,
 CONSTRAINT [PK_Analog_Input_Err_id] PRIMARY KEY CLUSTERED 
(
	[id] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[Serial_Port_Data]    Script Date: 2020/3/29 13:07:23 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[Serial_Port_Data](
	[id] [bigint] IDENTITY(1,1) NOT NULL,
	[serialPort] [nvarchar](20) NOT NULL,
	[time] [datetime2](7) NOT NULL,
	[data] [varchar](200) NULL,
	[isUse] [bit] NOT NULL,
 CONSTRAINT [PK_Serial_Port_id] PRIMARY KEY CLUSTERED 
(
	[id] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
SET ANSI_PADDING ON

GO
/****** Object:  Index [IX_Analog_Input_groupID]    Script Date: 2020/3/29 13:07:23 ******/
CREATE NONCLUSTERED INDEX [IX_Analog_Input_groupID] ON [dbo].[Analog_Input]
(
	[groupID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
/****** Object:  Index [IX_Analog_Input_startTime]    Script Date: 2020/3/29 13:07:23 ******/
CREATE NONCLUSTERED INDEX [IX_Analog_Input_startTime] ON [dbo].[Analog_Input]
(
	[startTime] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
/****** Object:  Index [IX_Analog_Input_Err_startTime]    Script Date: 2020/3/29 13:07:23 ******/
CREATE NONCLUSTERED INDEX [IX_Analog_Input_Err_startTime] ON [dbo].[Analog_Input_Err]
(
	[startTime] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
/****** Object:  Index [IX_Serial_Port_time]    Script Date: 2020/3/29 13:07:23 ******/
CREATE NONCLUSTERED INDEX [IX_Serial_Port_time] ON [dbo].[Serial_Port_Data]
(
	[time] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
ALTER TABLE [dbo].[Analog_Input] ADD  CONSTRAINT [DF_Analog_Input_isUse]  DEFAULT ((1)) FOR [isUse]
GO
ALTER TABLE [dbo].[Analog_Input_Err] ADD  CONSTRAINT [DF_Analog_Input_Err_isUse]  DEFAULT ((1)) FOR [isUse]
GO
ALTER TABLE [dbo].[Serial_Port_Data] ADD  CONSTRAINT [DF_Serial_Port_Data_isUse]  DEFAULT ((1)) FOR [isUse]
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'唯一标识，自增' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Analog_Input', @level2type=N'COLUMN',@level2name=N'id'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'用于标识每组动作的唯一标识，程序生成uuid' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Analog_Input', @level2type=N'COLUMN',@level2name=N'groupID'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'采集卡采集通道序号' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Analog_Input', @level2type=N'COLUMN',@level2name=N'channel'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'本次采集开始时间' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Analog_Input', @level2type=N'COLUMN',@level2name=N'startTime'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'本次采集频率' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Analog_Input', @level2type=N'COLUMN',@level2name=N'frequency'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'本次采集数据样本数量' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Analog_Input', @level2type=N'COLUMN',@level2name=N'sampleCount'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'本次采样数据' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Analog_Input', @level2type=N'COLUMN',@level2name=N'sampleData'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'数据是否过期，用于逻辑删除' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Analog_Input', @level2type=N'COLUMN',@level2name=N'isUse'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'字段含义与Analog_Input相同，但本表用于保存异常数据，例如设备故障时，通道采集数据超过预定时间，认为数据异常，将停止采集本次数据，并将已采集到的数据保存到本表' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Analog_Input_Err', @level2type=N'COLUMN',@level2name=N'id'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'主键，唯一标识' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Serial_Port_Data', @level2type=N'COLUMN',@level2name=N'id'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'485端口序号' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Serial_Port_Data', @level2type=N'COLUMN',@level2name=N'serialPort'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'数据采集时间' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Serial_Port_Data', @level2type=N'COLUMN',@level2name=N'time'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'采集的数据内容' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Serial_Port_Data', @level2type=N'COLUMN',@level2name=N'data'
GO
EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'数据是否过期，用于逻辑删除' , @level0type=N'SCHEMA',@level0name=N'dbo', @level1type=N'TABLE',@level1name=N'Serial_Port_Data', @level2type=N'COLUMN',@level2name=N'isUse'
GO
USE [master]
GO
ALTER DATABASE [CollectionDB] SET  READ_WRITE 
GO
