using System.ComponentModel.DataAnnotations.Schema;
using System.ComponentModel.DataAnnotations;
using System.Diagnostics.CodeAnalysis;

namespace SFWebAPI.Models
{
    public class Player
    {
        [Key]
        [NotNull]
        [Column(TypeName = "BIGINT")]
        public int PlayerId { get; set; }

        [Column(TypeName = "BIGINT")]
        public int? UserId { get; set; }


        [Column(TypeName = "SMALLINT")]
        public int? Level { get; set; }

        [Column(TypeName = "SMALLINT")]
        public int? Experience { get; set; }
        [Column(TypeName = "SMALLINT")]
        public int? RequiredExp { get; set; }

        [Column(TypeName = "SMALLINT")]
        public int? GameMoney { get; set; }
        [Column(TypeName = "SMALLINT")]
        public int? RealMoney { get; set; }

        [Column(TypeName = "SMALLINT")]
        public int? TotalGameCount { get; set; }
        [Column(TypeName = "SMALLINT")]
        public int? WinGameCount { get; set; }

        public bool? IsLevel1Clear{ get; set; }
        public bool? IsLevel2Clear { get; set; }
        public bool? IsLevel3Clear { get; set; }

        //List<int>? OwnedItemId { get; set; }
        //List<int>? OwnedCharacterId { get; set; }
    }
}
