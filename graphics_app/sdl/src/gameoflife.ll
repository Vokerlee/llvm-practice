; ModuleID = 'src/gameoflife.c'
source_filename = "src/gameoflife.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @gameoflife_start() #0 {
  %1 = alloca [2 x [720 x [1080 x i32]]], align 16
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  store i32 0, ptr %2, align 4
  %12 = load i32, ptr %2, align 4
  %13 = sub nsw i32 1, %12
  %14 = sext i32 %13 to i64
  %15 = getelementptr inbounds [2 x [720 x [1080 x i32]]], ptr %1, i64 0, i64 %14
  %16 = getelementptr inbounds [720 x [1080 x i32]], ptr %15, i64 0, i64 0
  call void @gameoflife_init(ptr noundef %16)
  store i32 100000, ptr %3, align 4
  store i32 0, ptr %4, align 4
  br label %17

17:                                               ; preds = %164, %0
  %18 = load i32, ptr %4, align 4
  %19 = load i32, ptr %3, align 4
  %20 = icmp slt i32 %18, %19
  br i1 %20, label %21, label %167

21:                                               ; preds = %17
  store i32 0, ptr %5, align 4
  br label %22

22:                                               ; preds = %153, %21
  %23 = load i32, ptr %5, align 4
  %24 = icmp slt i32 %23, 720
  br i1 %24, label %25, label %156

25:                                               ; preds = %22
  store i32 0, ptr %6, align 4
  br label %26

26:                                               ; preds = %149, %25
  %27 = load i32, ptr %6, align 4
  %28 = icmp slt i32 %27, 1080
  br i1 %28, label %29, label %152

29:                                               ; preds = %26
  store i32 0, ptr %7, align 4
  store i32 -1, ptr %8, align 4
  br label %30

30:                                               ; preds = %75, %29
  %31 = load i32, ptr %8, align 4
  %32 = icmp sle i32 %31, 1
  br i1 %32, label %33, label %78

33:                                               ; preds = %30
  store i32 -1, ptr %9, align 4
  br label %34

34:                                               ; preds = %71, %33
  %35 = load i32, ptr %9, align 4
  %36 = icmp sle i32 %35, 1
  br i1 %36, label %37, label %74

37:                                               ; preds = %34
  %38 = load i32, ptr %9, align 4
  %39 = icmp eq i32 %38, 0
  br i1 %39, label %40, label %44

40:                                               ; preds = %37
  %41 = load i32, ptr %8, align 4
  %42 = icmp eq i32 %41, 0
  br i1 %42, label %43, label %44

43:                                               ; preds = %40
  br label %71

44:                                               ; preds = %40, %37
  %45 = load i32, ptr %2, align 4
  %46 = sub nsw i32 1, %45
  %47 = sext i32 %46 to i64
  %48 = getelementptr inbounds [2 x [720 x [1080 x i32]]], ptr %1, i64 0, i64 %47
  %49 = load i32, ptr %5, align 4
  %50 = load i32, ptr %8, align 4
  %51 = add nsw i32 %49, %50
  %52 = add nsw i32 %51, 720
  %53 = srem i32 %52, 720
  %54 = sext i32 %53 to i64
  %55 = getelementptr inbounds [720 x [1080 x i32]], ptr %48, i64 0, i64 %54
  %56 = load i32, ptr %6, align 4
  %57 = load i32, ptr %9, align 4
  %58 = add nsw i32 %56, %57
  %59 = add nsw i32 %58, 1080
  %60 = srem i32 %59, 1080
  %61 = sext i32 %60 to i64
  %62 = getelementptr inbounds [1080 x i32], ptr %55, i64 0, i64 %61
  %63 = load i32, ptr %62, align 4
  store i32 %63, ptr %10, align 4
  %64 = load i32, ptr %10, align 4
  %65 = and i32 %64, -16777216
  %66 = icmp ne i32 %65, 0
  br i1 %66, label %67, label %70

67:                                               ; preds = %44
  %68 = load i32, ptr %7, align 4
  %69 = add nsw i32 %68, 1
  store i32 %69, ptr %7, align 4
  br label %70

70:                                               ; preds = %67, %44
  br label %71

71:                                               ; preds = %70, %43
  %72 = load i32, ptr %9, align 4
  %73 = add nsw i32 %72, 1
  store i32 %73, ptr %9, align 4
  br label %34, !llvm.loop !6

74:                                               ; preds = %34
  br label %75

75:                                               ; preds = %74
  %76 = load i32, ptr %8, align 4
  %77 = add nsw i32 %76, 1
  store i32 %77, ptr %8, align 4
  br label %30, !llvm.loop !8

78:                                               ; preds = %30
  %79 = load i32, ptr %2, align 4
  %80 = sub nsw i32 1, %79
  %81 = sext i32 %80 to i64
  %82 = getelementptr inbounds [2 x [720 x [1080 x i32]]], ptr %1, i64 0, i64 %81
  %83 = load i32, ptr %5, align 4
  %84 = sext i32 %83 to i64
  %85 = getelementptr inbounds [720 x [1080 x i32]], ptr %82, i64 0, i64 %84
  %86 = load i32, ptr %6, align 4
  %87 = sext i32 %86 to i64
  %88 = getelementptr inbounds [1080 x i32], ptr %85, i64 0, i64 %87
  %89 = load i32, ptr %88, align 4
  store i32 %89, ptr %11, align 4
  %90 = load i32, ptr %11, align 4
  %91 = and i32 %90, -16777216
  %92 = icmp ne i32 %91, 0
  br i1 %92, label %93, label %109

93:                                               ; preds = %78
  %94 = load i32, ptr %7, align 4
  %95 = icmp slt i32 %94, 2
  br i1 %95, label %99, label %96

96:                                               ; preds = %93
  %97 = load i32, ptr %7, align 4
  %98 = icmp sgt i32 %97, 3
  br i1 %98, label %99, label %109

99:                                               ; preds = %96, %93
  %100 = load i32, ptr %2, align 4
  %101 = sext i32 %100 to i64
  %102 = getelementptr inbounds [2 x [720 x [1080 x i32]]], ptr %1, i64 0, i64 %101
  %103 = load i32, ptr %5, align 4
  %104 = sext i32 %103 to i64
  %105 = getelementptr inbounds [720 x [1080 x i32]], ptr %102, i64 0, i64 %104
  %106 = load i32, ptr %6, align 4
  %107 = sext i32 %106 to i64
  %108 = getelementptr inbounds [1080 x i32], ptr %105, i64 0, i64 %107
  store i32 0, ptr %108, align 4
  br label %148

109:                                              ; preds = %96, %78
  %110 = load i32, ptr %11, align 4
  %111 = and i32 %110, -16777216
  %112 = icmp eq i32 %111, 0
  br i1 %112, label %113, label %126

113:                                              ; preds = %109
  %114 = load i32, ptr %7, align 4
  %115 = icmp eq i32 %114, 3
  br i1 %115, label %116, label %126

116:                                              ; preds = %113
  %117 = load i32, ptr %2, align 4
  %118 = sext i32 %117 to i64
  %119 = getelementptr inbounds [2 x [720 x [1080 x i32]]], ptr %1, i64 0, i64 %118
  %120 = load i32, ptr %5, align 4
  %121 = sext i32 %120 to i64
  %122 = getelementptr inbounds [720 x [1080 x i32]], ptr %119, i64 0, i64 %121
  %123 = load i32, ptr %6, align 4
  %124 = sext i32 %123 to i64
  %125 = getelementptr inbounds [1080 x i32], ptr %122, i64 0, i64 %124
  store i32 -16733645, ptr %125, align 4
  br label %147

126:                                              ; preds = %113, %109
  %127 = load i32, ptr %2, align 4
  %128 = sub nsw i32 1, %127
  %129 = sext i32 %128 to i64
  %130 = getelementptr inbounds [2 x [720 x [1080 x i32]]], ptr %1, i64 0, i64 %129
  %131 = load i32, ptr %5, align 4
  %132 = sext i32 %131 to i64
  %133 = getelementptr inbounds [720 x [1080 x i32]], ptr %130, i64 0, i64 %132
  %134 = load i32, ptr %6, align 4
  %135 = sext i32 %134 to i64
  %136 = getelementptr inbounds [1080 x i32], ptr %133, i64 0, i64 %135
  %137 = load i32, ptr %136, align 4
  %138 = load i32, ptr %2, align 4
  %139 = sext i32 %138 to i64
  %140 = getelementptr inbounds [2 x [720 x [1080 x i32]]], ptr %1, i64 0, i64 %139
  %141 = load i32, ptr %5, align 4
  %142 = sext i32 %141 to i64
  %143 = getelementptr inbounds [720 x [1080 x i32]], ptr %140, i64 0, i64 %142
  %144 = load i32, ptr %6, align 4
  %145 = sext i32 %144 to i64
  %146 = getelementptr inbounds [1080 x i32], ptr %143, i64 0, i64 %145
  store i32 %137, ptr %146, align 4
  br label %147

147:                                              ; preds = %126, %116
  br label %148

148:                                              ; preds = %147, %99
  br label %149

149:                                              ; preds = %148
  %150 = load i32, ptr %6, align 4
  %151 = add nsw i32 %150, 1
  store i32 %151, ptr %6, align 4
  br label %26, !llvm.loop !9

152:                                              ; preds = %26
  br label %153

153:                                              ; preds = %152
  %154 = load i32, ptr %5, align 4
  %155 = add nsw i32 %154, 1
  store i32 %155, ptr %5, align 4
  br label %22, !llvm.loop !10

156:                                              ; preds = %22
  %157 = load i32, ptr %2, align 4
  %158 = sext i32 %157 to i64
  %159 = getelementptr inbounds [2 x [720 x [1080 x i32]]], ptr %1, i64 0, i64 %158
  %160 = getelementptr inbounds [720 x [1080 x i32]], ptr %159, i64 0, i64 0
  call void @sgl_update(ptr noundef %160)
  %161 = load i32, ptr %2, align 4
  %162 = add nsw i32 %161, 1
  %163 = srem i32 %162, 2
  store i32 %163, ptr %2, align 4
  br label %164

164:                                              ; preds = %156
  %165 = load i32, ptr %4, align 4
  %166 = add nsw i32 %165, 1
  store i32 %166, ptr %4, align 4
  br label %17, !llvm.loop !11

167:                                              ; preds = %17
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal void @gameoflife_init(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store ptr %0, ptr %2, align 8
  store i32 -21731286, ptr %3, align 4
  %6 = load i32, ptr %3, align 4
  call void @sgl_srand(i32 noundef %6)
  store i32 0, ptr %4, align 4
  br label %7

7:                                                ; preds = %29, %1
  %8 = load i32, ptr %4, align 4
  %9 = icmp slt i32 %8, 720
  br i1 %9, label %10, label %32

10:                                               ; preds = %7
  store i32 0, ptr %5, align 4
  br label %11

11:                                               ; preds = %25, %10
  %12 = load i32, ptr %5, align 4
  %13 = icmp slt i32 %12, 1080
  br i1 %13, label %14, label %28

14:                                               ; preds = %11
  %15 = call i32 (...) @sgl_rand()
  %16 = srem i32 %15, 2
  %17 = mul i32 -16733645, %16
  %18 = load ptr, ptr %2, align 8
  %19 = load i32, ptr %4, align 4
  %20 = sext i32 %19 to i64
  %21 = getelementptr inbounds [1080 x i32], ptr %18, i64 %20
  %22 = load i32, ptr %5, align 4
  %23 = sext i32 %22 to i64
  %24 = getelementptr inbounds [1080 x i32], ptr %21, i64 0, i64 %23
  store i32 %17, ptr %24, align 4
  br label %25

25:                                               ; preds = %14
  %26 = load i32, ptr %5, align 4
  %27 = add nsw i32 %26, 1
  store i32 %27, ptr %5, align 4
  br label %11, !llvm.loop !12

28:                                               ; preds = %11
  br label %29

29:                                               ; preds = %28
  %30 = load i32, ptr %4, align 4
  %31 = add nsw i32 %30, 1
  store i32 %31, ptr %4, align 4
  br label %7, !llvm.loop !13

32:                                               ; preds = %7
  %33 = load ptr, ptr %2, align 8
  call void @sgl_update(ptr noundef %33)
  ret void
}

declare void @sgl_update(ptr noundef) #1

declare void @sgl_srand(i32 noundef) #1

declare i32 @sgl_rand(...) #1

attributes #0 = { noinline nounwind optnone sspstrong uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 16.0.6"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
!10 = distinct !{!10, !7}
!11 = distinct !{!11, !7}
!12 = distinct !{!12, !7}
!13 = distinct !{!13, !7}
