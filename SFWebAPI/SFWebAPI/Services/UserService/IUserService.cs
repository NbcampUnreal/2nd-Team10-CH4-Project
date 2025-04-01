
namespace SFWebAPI.Services.UserService
{
    public interface IUserService
    {

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
