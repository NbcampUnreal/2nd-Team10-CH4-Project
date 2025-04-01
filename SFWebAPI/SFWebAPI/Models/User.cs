using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Metadata.Internal;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Diagnostics.CodeAnalysis;

namespace SFWebAPI.Models
{
    [Index(nameof(Account), IsUnique = true)]

    public class User
    {
        [Key]
        [NotNull]
        [Column(TypeName = "BIGINT")]
        public int Id { get; set; }

        [Required]
        [Column(TypeName = "VARCHAR")]
        [StringLength(50)]
        public string? Account { get; set; }

        [Column(TypeName = "VARCHAR")]
        [StringLength(50)]
        public string? Password { get; set; }
    }
}
