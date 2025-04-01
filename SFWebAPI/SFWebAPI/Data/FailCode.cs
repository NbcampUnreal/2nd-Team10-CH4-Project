namespace SFWebAPI.Data
{
    public enum FailCode
    {
        SessionIDNotFound = 1000,        // Request 시 SessionIDfmf 못찾았을 경우.
        AccountNotFound = 1001,          // 아이디를 찾을 수 없음.
        PasswordMismatched = 1002,       // 패스워드가 틀렸음.
        AccountExist = 1003,             // 동일한 아이디가 있음.
        AlreadyLogIn = 1004,             // 이미 로그인 되어있음.
    }
}
