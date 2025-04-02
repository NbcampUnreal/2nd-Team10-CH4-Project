namespace SFWebAPI.Core
{
    public static class Extension
    {
        public static T ToEnum<T>(this string enumString)
        {
            return (T)Enum.Parse(typeof(T), enumString);
        }

        public static T ToEnum<T>(this int value)
        {
            if (!typeof(T).IsEnum)
            {
                return default(T);
                throw new ArgumentException("T must be an enumerated type");
            }

            if (!Enum.IsDefined(typeof(T), value))
            {
                return default(T);
                throw new ArgumentOutOfRangeException(nameof(value), "Invalid enum value");
            }

            return (T)Enum.ToObject(typeof(T), value);
        }
    }
}

