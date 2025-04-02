namespace SFWebAPI.RequestBody
{
    public class RequestCreateAccount
    {
        public string? Account { get; set; }
        public string? Password { get; set; }
    }

    public class RequestLogIn
    {
        public string? Account { get; set; }
        public string? Password { get; set; }
    }

    public class RequestLogOut
    {
        public string? SessionId { get; set; }
    }

    public class RequestGetPlayerInfo
    {
        public string? SessionId { get; set; }
    }

    public class RequestUpdatePlayerInfo
    {
        public string? SessionId { get; set; }
        public int? ExperienceAmount { get; set; }
        public int? GameMoneyAmount { get; set; }
    }

    public class RequestAddExpirence
    {
        public string? SessionId { get; set; }
        public int? Amount { get; set; }
    }

    public class RequestAddGameMoney
    {
        public string? SessionId { get; set; }
        public int? Amount { get; set; }
    }

    public class RequestAddRealMoney
    {
        public string? SessionId { get; set; }
        public int? Amount { get; set; }
    }
}
