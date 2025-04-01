using SFWebAPI.Services.UserService;
using Microsoft.AspNetCore.Mvc;

namespace SFWebAPI.Controllers;

[ApiController]
[Route("api/[controller]")]
public class UserController : ControllerBase
{
    private readonly IUserService _userService;
    public UserController(IUserService usersService)
    {
        _userService = usersService;
    }

    [HttpPost("CreateAccount")]
    public async Task<ActionResult<ResponseBody<string>>> AddUser(RequestCreateAccount request)
    {
        var result = await _userService.CreateAccount(request);
        return Ok(result);
    }

    [HttpPost("LogIn")]
    public async Task<ActionResult<ResponseBody<SessionData>>> LogInUser(RequestLogIn request)
    {
        var result = await _userService.LogInUser(request);
        return Ok(result);
    }

    [HttpPost("LogOut")]
    public async Task<ActionResult<ResponseBody<SessionData>>> LogOutUser(RequestLogOut request)
    {
        var result = await _userService.LogOutUser(request);
        return Ok(result);
    }


    //--------------------------------- Get All Users ---------------------------------
    [HttpGet]
    public async Task<ActionResult<List<User>>> GetAllUsers()
    {
        return await _userService.GetAllUsers();
    }

    // ----------------------------------- Post ----------------------------------------------
    [HttpPost]
    public async Task<ActionResult<User>> DefaultAddUser(User user)
    {
        //var httpHeaders = HttpContext.Request.Headers;
        //foreach (var header in httpHeaders)
        //{
        //    Console.WriteLine($"{header.Key}, {header.Value}");
        //}

        var result = await _userService.DefaultAddUser(user);
        if (result is null)
            return NotFound("Users not found.");

        return Ok(result);
    }


    // ----------------------------------- Put ----------------------------------------------
    [HttpPut("{id}")]
    public async Task<ActionResult<User>> UpdateUser(int id, User request)
    {
        var result = await _userService.UpdateUser(id, request);
        if (result is null)
            return NotFound("Users not found.");

        return Ok(result);
    }

    // ----------------------------------- Delete -------------------------------------------
    [HttpDelete("{id}")]
    public async Task<ActionResult<List<User>>> DefaultDeleteUser(int id)
    {
        var result = await _userService.DefaultDeleteUser(id);
        if (result is null)
            return NotFound("Users not found.");

        return Ok(result);
    }
}
