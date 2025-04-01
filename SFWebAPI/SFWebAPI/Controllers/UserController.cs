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

    //private static readonly string[] Summaries = new[]
    //{
    //    "Freezing", "Bracing", "Chilly", "Cool", "Mild", "Warm", "Balmy", "Hot", "Sweltering", "Scorching"
    //};

    //private readonly ILogger<WeatherForecastController> _logger;

    //public WeatherForecastController(ILogger<WeatherForecastController> logger)
    //{
    //    _logger = logger;
    //}

    //[HttpGet(Name = "GetWeatherForecast")]
    //public IEnumerable<WeatherForecast> Get()
    //{
    //    return Enumerable.Range(1, 5).Select(index => new WeatherForecast
    //    {
    //        Date = DateOnly.FromDateTime(DateTime.Now.AddDays(index)),
    //        TemperatureC = Random.Shared.Next(-20, 55),
    //        Summary = Summaries[Random.Shared.Next(Summaries.Length)]
    //    })
    //    .ToArray();
    //}
}
