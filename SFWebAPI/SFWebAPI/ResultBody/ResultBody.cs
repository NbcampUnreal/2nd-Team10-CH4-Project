namespace SFWebAPI.ResultBody
{
    public class ResponseBody<TData>
    {
        public int ResultCode { get; set; }
        public string? ResultMessage { get; set; }

        public TData? Data { get; set; }
    }
}
