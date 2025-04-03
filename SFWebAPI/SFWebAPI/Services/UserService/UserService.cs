using Microsoft.EntityFrameworkCore;
using SFWebAPI.Models;
using System.Diagnostics;
using System.Net;

namespace SFWebAPI.Services.UserService
{
    public class UserService : IUserService
    {
        private readonly DataContext _context;

        public UserService(DataContext context)
        {
            _context = context;
        }

        public async Task<ResponseBody<string>> CreateAccount(RequestCreateAccount request)
        {
            var responseBody = new ResponseBody<string>();

            // 계정 존재 여부 확인
            var checkUserAccount = await _context.Users.AnyAsync(x => x.Account == request.Account);
            if (checkUserAccount)
            {
                responseBody.ResultCode = (int)FailCode.AccountExist;
                responseBody.ResultMessage = "Account Already Exit";
                return responseBody;
            }

            var user = new User
            {
                Account = request.Account,
                Password = request.Password
            };

            _context.Users.Add(user);
            await _context.SaveChangesAsync();

            var addedUser = _context.Users.Where(x => x.Account == request.Account).FirstOrDefault();
            if(addedUser is not null)
            {
                var newPlayer = new Player
                {
                    UserId = addedUser.UserId,
                    Level = 1,
                    Experience = 0,
                    RequiredExp = 100,
                    GameMoney = 0,
                    RealMoney = 0,
                    TotalGameCount = 0,
                    WinGameCount = 0,
                    IsLevel1Clear = false,
                    IsLevel2Clear = false,
                    IsLevel3Clear = false
                };

                _context.Players.Add(newPlayer);
                await _context.SaveChangesAsync();
            }

            responseBody.ResultCode = (int)HttpStatusCode.OK;
            responseBody.ResultMessage = "OK";
            responseBody.Data = "OK";

            return responseBody;
        }


        public async Task<ResponseBody<SessionData>> LogInUser(RequestLogIn request)
        {
            var responseBody = new ResponseBody<SessionData>();

            // 계정 존재 여부 확인
            var user = await _context.Users.FirstOrDefaultAsync(x => x.Account == request.Account);
            if (user == null)
            {
                responseBody.ResultCode = (int)FailCode.AccountNotFound;
                responseBody.ResultMessage = "Account not found";
                return responseBody;
            }

            // 패스워드 일치여부 확인
            var isPasswordMatched = await _context.Users.AnyAsync(x => x.Account == request.Account && x.Password == request.Password);
            if (!isPasswordMatched)
            {
                responseBody.ResultCode = (int)FailCode.PasswordMismatched;
                responseBody.ResultMessage = "Password mismatched";
                return responseBody;
            }

            var newSession = SessionManager.Instance.GenerateSession();

            responseBody.ResultCode = (int)HttpStatusCode.OK;
            responseBody.ResultMessage = "Ok";
            responseBody.Data = newSession;

            SessionManager.Instance.AddSession(newSession, request.Account);
            return responseBody;
        }

        public async Task<ResponseBody<SessionData>> LogOutUser(RequestLogOut request)
        {
            var responseBody = new ResponseBody<SessionData>();
            SessionData sessionData = new SessionData
            {
                SessionId = request.SessionId
            };

            // 굉장히 간략화함
            await Task.Run(() => SessionManager.Instance.RemoveSession(sessionData));

            responseBody.ResultCode = (int)HttpStatusCode.OK;
            responseBody.ResultMessage = "Succeess LogOut By SessionId";
            responseBody.Data = sessionData;

            return responseBody;
        }


        #region Default CRUD
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
        #endregion
    }
}

