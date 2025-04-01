using Microsoft.EntityFrameworkCore;

namespace SFWebAPI.Services.UserService
{
    public class UserService : IUserService
    {
        private readonly DataContext _context;

        public UserService(DataContext context)
        {
            _context = context;
        }


        public async Task<List<User>> GetAllUsers()
        {
            var users = await _context.Users.ToListAsync();
            return users;
        }

        public async Task<User> DefaultAddUser(User user)
        {
            _context.Users.Add(user);
            await _context.SaveChangesAsync();

            return user;
        }

        public async Task<User?> UpdateUser(int id, User request)
        {
            // Get Databases
            var user = await _context.Users.FindAsync(id);
            if (user is null)
                return null;

            // Change Databases
            user.Account = request.Account;
            user.Password = request.Password;

            // Save Changes async
            await _context.SaveChangesAsync();

            return await _context.Users.FindAsync(id);
        }

        public async Task<List<User>?> DefaultDeleteUser(int id)
        {
            var user = await _context.Users.FindAsync(id);
            if (user is null)
                return null;

            _context.Users.Remove(user);
            await _context.SaveChangesAsync();

            return await _context.Users.ToListAsync();
        }

    }
}

