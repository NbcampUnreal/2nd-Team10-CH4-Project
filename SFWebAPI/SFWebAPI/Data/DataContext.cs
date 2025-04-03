using Microsoft.EntityFrameworkCore;


namespace SFWebAPI.Data
{
    public class DataContext : DbContext
    {
        public DataContext(DbContextOptions<DataContext> options) : base(options)
        {

        }

        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {
            var connectionString = WebApplication.CreateBuilder().Configuration.GetConnectionString("DefaultConnection");

            base.OnConfiguring(optionsBuilder);
            optionsBuilder.UseMySql(connectionString, ServerVersion.AutoDetect(connectionString));
        }

        public DbSet<User> Users { get; set; }
        public DbSet<Player> Players { get; set; }
    }
}
