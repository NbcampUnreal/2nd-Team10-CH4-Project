namespace SFWebAPI.ResultBody
{
    public class ResultLogIn
    {
        public string? SessionId { get; set; }
    }

    public class ResultGetPlayerInfo
    {
        public string? Account { get; set; }
        public int? GameMoney { get; set; }
        public int? RealMoney { get; set; }

        //List<int>? OwnedItemId { get; set; }
        //List<int>? OwnedCharacterId { get; set; }
    }
}
