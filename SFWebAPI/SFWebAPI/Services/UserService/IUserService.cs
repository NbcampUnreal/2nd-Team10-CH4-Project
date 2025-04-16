
namespace SFWebAPI.Services.UserService
{
    public interface IUserService
    {
        Task<ResponseBody<string>> CreateAccount(RequestCreateAccount request);

        Task<ResponseBody<SessionData>> LogInUser(RequestLogIn request);

        Task<ResponseBody<SessionData>> LogOutUser(RequestLogOut request);

        #region Default CRUD
        // Get All Users 
        Task<List<User>> GetAllUsers();

        // POST User DB
        Task<User> DefaultAddUser(User user);

        // Update User DB
        Task<User?> UpdateUser(int id, User request);

        // Drop User DB
        Task<List<User>?> DefaultDeleteUser(int id);
        #endregion
    }
}
