using Microsoft.EntityFrameworkCore.Metadata;
using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace SFWebAPI.Migrations
{
    /// <inheritdoc />
    public partial class Initial : Migration
    {
        /// <inheritdoc />
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.AlterDatabase()
                .Annotation("MySql:CharSet", "utf8mb4");

            migrationBuilder.CreateTable(
                name: "Players",
                columns: table => new
                {
                    PlayerId = table.Column<long>(type: "BIGINT", nullable: false)
                        .Annotation("MySql:ValueGenerationStrategy", MySqlValueGenerationStrategy.IdentityColumn),
                    UserId = table.Column<long>(type: "BIGINT", nullable: true),
                    Level = table.Column<short>(type: "SMALLINT", nullable: true),
                    Experience = table.Column<short>(type: "SMALLINT", nullable: true),
                    RequiredExp = table.Column<short>(type: "SMALLINT", nullable: true),
                    GameMoney = table.Column<short>(type: "SMALLINT", nullable: true),
                    RealMoney = table.Column<short>(type: "SMALLINT", nullable: true),
                    TotalGameCount = table.Column<short>(type: "SMALLINT", nullable: true),
                    WinGameCount = table.Column<short>(type: "SMALLINT", nullable: true),
                    IsLevel1Clear = table.Column<bool>(type: "tinyint(1)", nullable: true),
                    IsLevel2Clear = table.Column<bool>(type: "tinyint(1)", nullable: true),
                    IsLevel3Clear = table.Column<bool>(type: "tinyint(1)", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Players", x => x.PlayerId);
                })
                .Annotation("MySql:CharSet", "utf8mb4");

            migrationBuilder.CreateTable(
                name: "Users",
                columns: table => new
                {
                    UserId = table.Column<long>(type: "BIGINT", nullable: false)
                        .Annotation("MySql:ValueGenerationStrategy", MySqlValueGenerationStrategy.IdentityColumn),
                    Account = table.Column<string>(type: "VARCHAR(50)", maxLength: 50, nullable: false)
                        .Annotation("MySql:CharSet", "utf8mb4"),
                    Password = table.Column<string>(type: "VARCHAR(50)", maxLength: 50, nullable: true)
                        .Annotation("MySql:CharSet", "utf8mb4")
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Users", x => x.UserId);
                })
                .Annotation("MySql:CharSet", "utf8mb4");

            migrationBuilder.CreateIndex(
                name: "IX_Users_Account",
                table: "Users",
                column: "Account",
                unique: true);
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropTable(
                name: "Players");

            migrationBuilder.DropTable(
                name: "Users");
        }
    }
}
