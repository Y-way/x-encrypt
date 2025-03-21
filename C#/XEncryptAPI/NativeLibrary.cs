public static class NativeLibrary
{
#if UNITY_EDITOR
    public const string Name = "XEncrypt";
#else
#if UNITY_STANDALONE_WIN || UNITY_STANDALONE_OSX || UNITY_STANDALONE_LINUX
    public const string Name = "XEncrypt";
#elif UNITY_IOS
    public const string Name = "__Internal";
#elif UNITY_ANDROID
    public const string Name = "__Internal";
#else
    public const string Name = "XEncrypt";
#endif
#endif
}
